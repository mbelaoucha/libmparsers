#ifndef M_STRING_UTILS_HEADER
#define M_STRING_UTILS_HEADER

/*c++ std includes*/
#include <vector>
#include <iostream>

namespace mparsers
{
    /** \defgroup mtools_string String utils
     * \brief A set function of useful string related operations
     * \{
     * \file mstring_utils.h
     * \brief some useful stuffs to handle strings
     * \author Marouane BELAOUCHA
     */


    /** \brief split a string
     * \param [in] str: string to split
     * \param [int] delimiter: delimiter  character
     * \return a vector of split parts
     */
    std::vector<std::string> split(const std::string &str, char delimiter);

    /** \brief ingore a comment part in a string
     * \param [in] str: string process
     * \param [int] comment_char: comment starter character
     * \return uncommented string
     */
    std::string ignore_comment(const std::string& str, char comment_char);

    /** \brief trim a C string
     * \param [in,out] s: string to trim
     * \return trimmed string
     */
    char* strtrim(char* s);

    /** \brief trim a string
     * \param [in] str: string to trim
     * \return trimmed string
     */
    std::string stdstrim(const std::string& str);

    /** \brief build a safe file name from any given random string
     * the process consists on replacing every unsafe character by '_'. Safe characters are alpha-numeral and/or '_'.
     * \param [in] str: input string
     * \return safe base which can be used as a file name
     */
    std::string build_filename(const std::string& str);


    std::string splitpath(const std::string& filename);
    std::string splitext(const std::string& filename);

} /* namespace mparsers */

/**
* \}
*/
#endif /*MERLIN_STRING_UTILS_HEADER*/
