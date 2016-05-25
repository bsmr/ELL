////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     SweepingIncrementalTrainer.tcc (trainers)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// utilities
#include "RandomEngines.h"

namespace trainers
{
    template <typename PredictorType>
    SweepingIncrementalTrainer<PredictorType>::SweepingIncrementalTrainer(std::unique_ptr<IIncrementalTrainer<PredictorType>>&& incrementalTrainer, const SweepingIncrementalTrainerParameters& parameters, std::shared_ptr<evaluators::IEvaluator<PredictorType>> evaluator) :
        _incrementalTrainer(std::move(incrementalTrainer)), _parameters(parameters), _evaluator(std::move(evaluator)), _random(utilities::GetRandomEngine(parameters.dataPermutationRandomSeed))
    {}
    
    template <typename PredictorType>
    void SweepingIncrementalTrainer<PredictorType>::Update(dataset::GenericRowDataset::Iterator exampleIterator)
    {
        dataset::GenericRowDataset rowDataset(exampleIterator);

        // calculate epoch size
        uint64_t epochSize = _parameters.epochSize;
        if(epochSize == 0 || epochSize >  rowDataset.NumExamples())
        {
            epochSize = rowDataset.NumExamples();
        }

        for(int epoch = 0; epoch < _parameters.numEpochs; ++epoch)
        {
            // randomly permute the data
            rowDataset.RandomPermute(_random, epochSize);

            // update the incremental trainer
            auto trainSetIterator = rowDataset.GetIterator(0, epochSize);
            _incrementalTrainer->Update(trainSetIterator);

            // evaluate
            if (_evaluator != nullptr)
            {
                _evaluator->Evaluate(*_incrementalTrainer->GetPredictor());
            }
        }
    }

    template <typename PredictorType>
    std::unique_ptr<IIncrementalTrainer<PredictorType>> MakeSweepingIncrementalTrainer(std::unique_ptr<IIncrementalTrainer<PredictorType>>&& incrementalTrainer, const SweepingIncrementalTrainerParameters& parameters, std::shared_ptr<evaluators::IEvaluator<PredictorType>> evaluator)
    {
        return std::make_unique<SweepingIncrementalTrainer<PredictorType>>(std::move(incrementalTrainer), parameters, std::move(evaluator));
    }
}