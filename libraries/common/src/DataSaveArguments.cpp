// DataSaveArguments.cpp

#include "DataSaveArguments.h"

#include <vector>

namespace common
{
    void ParsedDataSaveArguments::AddArgs(utilities::CommandLineParser& parser)
    {
        parser.AddOption(
            outputDataFile,
            "outputDataFile", 
            "odf",
            "Path to the output data file",
            "");
    }

    utilities::ParseResult ParsedDataSaveArguments::PostProcess(const utilities::CommandLineParser & parser)
    {
        std::vector<std::string> parseErrorMessages;

        return parseErrorMessages;
    }
}
