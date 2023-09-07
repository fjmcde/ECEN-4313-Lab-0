#include <fstream>
#include <iostream>
#include <string>
#include <getopt.h>
#include <vector>

/***************************
 *      Defined macros     *
 ***************************/

/* Print defines */
#define AUTHOR_NAME                 "\nFrank McDermott\n"
/* getopt/getopt_long defines */
#define CMD_SHORT_OPTIONS           "i:o:"
#define CMD_NO_MORE_OPTIONS         -1
/* Help filed defined macros */
#define HELP_FILE_PATH              "help/help.txt"
/* Error messages defines */
#define PARSE_CMD_ERROR             "ERROR: parseCMD() RETURNED: "
#define FILE_ERROR_MSG              "ERROR: FAILED TO OPEN FILE!\n"
#define HELP_FILE_ERROR_MSG         "ERROR: FAILED TO OPEN THE HELP FILE!\n"
#define UNKNOWN_OPTION_MSG          "See help: mysort --help\n"
/* Debug only */
#ifndef NDEBUG
    // Debug print function
    void debugPrint(std::string str)
    {
        std::cout << str << std::endl;
    }

    // Macro to envoke the debugprint function
    #define DEBUG_PRINT(str)    debugPrint(str)
#else
    // In release mode the debug print macro does nothing.
    // In effect, it functions as a "NO OP". It doesn't generate 
    // any machine code instructions and won't generate warnings.
    #define DEBUG_PRINT(str)    (void)0
#endif

/*********************************
 *      Forward Declarations     *
 *********************************/

/* Helper Functions */
int mySortHelp(void);
int printName(void);
int unknownOption(void);


/****************************
 *      Enumerations        *
 ****************************/

/** 
 * @brief Enumerated error codes.
 * @details An errorCode <= 0 is not an actual error, handle accordingly 
 **/
enum errorCodes
{
    noErrorHelp         = -2,   /* No Error: Help requested */
    noErrorName         = -1,   /* No Error: Name requested*/
    noError             = 0,    /* No errors */
    streamError         = 1,    /* Error: General std::fstream error */
    algorithmError      = 2,    /* Error: Invalid algorithm requested */
    noOutputFileError   = 3,    /* Error: No output file path provided */
    noInputFileError    = 4,    /* Error: No input file path provided */
    unknownOptError     = 5     /* Error: Unknown/no option provided */
};


/*********************************
 *      Function Definitions     *
 *********************************/

/** 
 * @brief Function to parse command-line arguments and options 
 * @param argc
 *      Argument count
 * @param argv 
 *      Argument vector
 * @param inFile 
 *      Input file stream object
 * @param outFile
 *      Output file stream object
 * @param algorithm 
 *      String to determine which sorting algorithm to call
 * @return
 *      Returns an enumerated error code (see errorCodes enumeration)
 **/ 
int parseCMD(int argc, char* argv[], std::string* inFile, 
            std::string* outFile, std::string* algorithm)
{
    // Long options for the getopt_long function
    // 
    // Documentation: 
    // https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Options.html
    static const option long_options[] = {
        /* option struct data members:
          name      has_arg             flag        val */
        {"name",    no_argument,        nullptr,    'n'},
        {"alg",     required_argument,  nullptr,    'a'},
        {"help",    no_argument,        nullptr,    'h'},
        {nullptr,   no_argument,        nullptr,    0}
    };

    // Valid --alg options
    const std::vector<std::string> validAlgorithms = {"merge", "quick"};

    // Parse all command-line options/arguments
    while(true)
    {
        // Option index stored by getopt_long
        int options_idx = 0;

        // Get the next option
        int option = getopt_long(argc, argv, CMD_SHORT_OPTIONS, long_options, &options_idx);

        // getopt_long will return -1 when all options have been parsed
        if(option == CMD_NO_MORE_OPTIONS)
        {
            break;
        }

        // Parse options/arguments
        switch(option)
        {
            case 'n':
                return printName();
            case 'i':
                *inFile = (std::string)optarg;
                break;
            case 'o':
                *outFile = (std::string)optarg;
                break;
            case 'a':
                *algorithm = (std::string)optarg;
                break;
            case 'h':
                return mySortHelp();
            case '?':
                // Unknown or missing options
                return unknownOption();
        }
    }

    // Check for input/output file option errors
    if(!inFile) { return errorCodes::noInputFileError; }
    else if(!outFile) { return errorCodes::noOutputFileError; }

    // Loop though valid algorithms vector
    for(size_t i = 0; i < validAlgorithms.size(); i++)
    {
        // A valid algorithm was provided on CMD line
        if(*algorithm == validAlgorithms.at(i))
        {
            return errorCodes::noError;
        }
    }

    // An invalid algorithm was requested
    return errorCodes::algorithmError;
}


/** 
 * @brief Given an input file stream an vector object, this function will
 *           read each line of the file and store them as vector elements 
 * @param inputFile
 *      Reference to std::ifstream object for the input file to read in
 * @param values
 *      Reference to std::vector object to store each line of the input file 
 **/ 
void parseInputFIle(std::ifstream& inputFile, std::vector<int>& values)
 {
    std::string line;

    while(std::getline(inputFile, line))
    {
        values.push_back(std::stoi(line));
    }
 }


/**
 * @brief Print's the authors name
 * 
 * @details The author's name is set by the AUTHOR_NAME macro
 * 
 * @return Return no error
 **/
int printName(void)
{
    std::cout << AUTHOR_NAME;
    return errorCodes::noErrorName;
}


/** 
 * @brief  Prints the help file
 * 
 * @return Returns an error if 
 **/
int mySortHelp(void)
{
    // Open the help file
    std::ifstream helpFile(HELP_FILE_PATH);

    // Check the stream for errors
    if(!helpFile)
    {
        std::cerr << HELP_FILE_ERROR_MSG;
        return errorCodes::streamError;
    }

    std::string line;

    // print each line of the help file
    while(std::getline(helpFile, line))
    {
        std::cout << line << std::endl;
    }

    // Close the help file
    helpFile.close();

    return errorCodes::noErrorHelp;
}


/**
 * @brief Prints the unknown option mesage
 * 
 * @return An error code
 **/
int unknownOption(void)
{
    std::cout << UNKNOWN_OPTION_MSG;
    return errorCodes::unknownOptError;
}


/**
 * @brief Helper function for the quickSort function
 * 
 * @param subVector 
 *      The sub-vector to partition
 * @param startIndex
 *      The starting index of the sub-vector
 * @param endIndex 
 *      The ending index of the sub-vector
 * @return An error code 
 **/
int partitionVector(std::vector<int>& subVector, int startIndex, int endIndex)
{
    // Set the pivot value and pivot index
    int pivotValue = subVector[endIndex];
    int pivotIndex = startIndex;

    // Iterate though and partition the vector into left and right
    // partitions. The left partition contains all values that are 
    // less-than or equal-to the pivot value. The right partition
    // contains all values that are greater-than the pivor value.
    for(int i = startIndex; i < endIndex; i++)
    {
        if(subVector[i] <= pivotValue)
        {
            std::swap(subVector[i], subVector[pivotIndex]);
            pivotIndex++;
        }
    }

    // Place the pivot element in the correct sorted position
    std::swap(subVector[endIndex], subVector[pivotIndex]);

    return pivotIndex;
}


/**
 * @brief Recursive QuickSort algorithm
 * 
 * @param vector
 *      Vector data to sort
 * @param startIndex 
 *      Starting index to sort from
 * @param endIndex 
 *      Ending index to sort up through
 * @return void
 **/
void quickSort(std::vector<int>& vector, int startIndex, int endIndex)
{
    // Base case to break recursion
    if(startIndex >= endIndex)
    {
        return;
    }

    // Recursively partition and sort the vector
    int pivotIndex = partitionVector(vector, startIndex, endIndex);
    quickSort(vector, startIndex, pivotIndex - 1);
    quickSort(vector, pivotIndex + 1, endIndex);
}


/**
 * @brief Helper function for mergeSort
 * 
 * @param masterVector
 *      The vector containing all of the data to sort
 * @param start
 *      The starting index which defines the left endpoint of the left sub-vector
 * @param middle
 *      The middle index which defines an endpoint of the sub-vectors
 * @param end 
 *      THe ending index which defines the right endpoint of the right sub-vector
 * @return void
 **/
void merge(std::vector<int>& masterVector, size_t start, size_t middle, size_t end)
{
    // Create sub-vectors using std::vector's range constructor
    // NOTE: vector.begin() returns an iterator which points to the first element
    //       in the vector. The range is [start, end), hence we need to +1 to 
    //       the endpoint to make it inclusive.
    // See (3): https://cplusplus.com/reference/vector/vector/vector/
    std::vector<int> leftSubVector(masterVector.begin() + start, masterVector.begin() + middle + 1);
    std::vector<int> rightSubVector(masterVector.begin() + middle + 1, masterVector.begin() + end + 1);

    // Variables to track indexes
    size_t leftIndex = 0;
    size_t rightIndex = 0; 
    size_t masterIndex = start;
    
    // Iterate through each index of both sub-vectors and compare the value stored
    // in the left sub-vector with the value stored in the right subvector. Merging 
    // whichever is smaller back into the "master vector". After the merge, increment
    // both the master index and the sub-vector.
    while((leftIndex < leftSubVector.size()) && (rightIndex < rightSubVector.size()))
    {
        if(leftSubVector[leftIndex] <= rightSubVector[rightIndex])
        {
            masterVector[masterIndex] = leftSubVector[leftIndex++];
        }
        else
        {
            masterVector[masterIndex] = rightSubVector[rightIndex++];
        }

        masterIndex++;
    }

    // Since the while loop iterates until one of the sub-vectors
    // merges all of it's elements; the other sub-vector will have
    // at least one (and potentially more) unmerged element(s).

    // Merge in any remaining indexes of the left sub-vector
    for( ; leftIndex < leftSubVector.size(); leftIndex++)
    {
        masterVector[masterIndex++] = leftSubVector[leftIndex];
    }


    // Merge in any remaining indexes of the right sub-vector
    for( ; rightIndex < rightSubVector.size(); rightIndex++)
    {
        masterVector[masterIndex++] = rightSubVector[rightIndex];
    }
}


/**
 * @brief Recursive MergeSort algorithm
 * 
 * @param vector
 *      The vector containing data to sort
 * @param startIndex
 *      The starting index to sort from
 * @param endIndex 
 *      The ending index to sort through
 * @return void 
 **/
void mergeSort(std::vector<int>& vector, size_t startIndex, size_t endIndex)
{
    // Base case to break recursion
    if(startIndex >= endIndex)
    {
        return;
    }

    // Calculate the middle index to recursively subdivide vector
    // into sub-vectors
    int middleIndex = startIndex + ((endIndex - startIndex) / 2);

    // Recursively merge
    mergeSort(vector, startIndex, middleIndex);
    mergeSort(vector, middleIndex + 1, endIndex);
    merge(vector, startIndex, middleIndex, endIndex);
}


void sort(std::string algorithm, std::vector<int>& values)
{
    // Call proper sorting algorithm
    if(algorithm == "merge")
    {
        mergeSort(values, 0, values.size() - 1);
    }
    else if(algorithm == "quick")
    {
        quickSort(values, 0, values.size() - 1);
    }
}


/**
 * @brief Writes the sorted vector to a file
 * 
 * @param outFile
 *      A std::ofstream object to write sorted vector data to
 * @param values
 *      Sorted vector data
 * @return void 
 **/
void writeOutSortedToFile(std::ofstream& outFile, std::vector<int> values)
{
    for(size_t i = 0; i < values.size(); i++)
    {
        outFile << values.at(i) << std::endl;
    }
}