#include "..\headers\main.h"


int main(int argc, char* argv[])
{
    // Validate number of command-line arguments
    if(argc < 2)
    {
        // Print the help file
        return mySortHelp();
    }

    // Ccommand line option strings
    std::string input      = "";
    std::string output     = "";
    std::string algorithm  = "";

    // Parse the command-line arguments
    int error = parseCMD(argc, argv, &input, &output, &algorithm);

    // Validate parseCMD code
    if(error < errorCodes::noError)
    {
        // When --help or --name is supplied on command-line
        // a psuedo-error is returned to short-circuit the parsing
        // logic. Handled here and returns no error from main
        return errorCodes::noError;
    }
    else if(error > errorCodes::noError)
    {
        std::cerr << PARSE_CMD_ERROR + error << std::endl;
        return error;
    }

    // File stream objects
    std::ifstream inputFile(input);
    std::ofstream outputFile(output);

    // Check for stream errors
    if(!inputFile || !outputFile)
    {
        std::cerr << FILE_ERROR_MSG;
        return errorCodes::streamError;
    }

    // Parse the input file and close stream
    std::vector<int> values;
    parseInputFIle(inputFile, values);
    inputFile.close();

    // Sort values using the requested algorithm
    sort(algorithm, values);

    // Write sorted values to file and close stream
    writeOutSortedToFile(outputFile, values);
    outputFile.close();
    
    return errorCodes::noError;
}
