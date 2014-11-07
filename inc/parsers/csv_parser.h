#ifndef CSV_UTILS_HEADER
#define CSV_UTILS_HEADER

#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include <misc/mstring_utils.h>



namespace mparsers
{

/**
 * \addtogroup parsers_group Parsers
 * @{
 */

 /**
 * \file csv_parser.h
 * \brief parser for .CVS (column separated files)
 * \author Marouane BELAOUCHA
 */

 /**
 * \brief This class wraps csv (column separated values) files, and grant access to its lines.
 * This class implements a traversal for csv files. user has to provide a callback function which
 * will be called each time the traversal matches a valid CSV line. This class makes CSV files management
 * as easy as the following example (with an anonymous function):

~~~~~~~~~~{.c}
#include <iostream>
#include <libmparsers.h>
using namespace std;
using namespace mparsers;

int main(int , char**)
{
    csv_parser_t<';','#'> my_csv_parser(3); // needs 3 columns and more
    my_csv_parser.each_row("my_input_file.csv",
                           [](const csv_parser_t<';','#'>::row_t& current_row, int current_line) -> void
    {
        cout<<"\line#"<<current_line<<": ";
        for(size_t it=0; it< current_row.size(); ++it)
            cout<<current_row[it]<<";";
    });
}
~~~~~~~~~~
Or, the same example with a C-like function as callback:

~~~~~~~~~~{.c}
#include <iostream>
#include <libmparsers.h>
using namespace std;
using namespace mparsers;

void call_this_back(const csv_parser_t<';','#'>::row_t& current_row, int current_line)
{
    cout<<"\line#"<<current_line<<": ";
    for(size_t it=0; it< current_row.size(); ++it)
        cout<<current_row[it]<<";";
}

int main(int , char**)
{
    csv_parser_t<';','#'> my_csv_parser(3); // needs 3 columns and more
    my_csv_parser.each_row('my_input_file.csv', call_this_back);
}
~~~~~~~~~~



 */
template <char __delimiter,char __comment_starter>
struct csv_parser_t
{
    /** \brief represents a row of a csv file
     */

    struct row_t
    {
        public:
            std::vector<std::string> fields;    //!< list of columns content
        public:
            row_t():fields(){}
            //!< @brief is the row empty?
            inline bool empty() const {return fields.empty();}
            //!< @brief return the number of columns
            inline int  size()  const {return fields.size();}
            //!< @brief clear columns
            inline void clear() {fields.clear();}
            //!< @brief grant read only random access to columns
            std::string& operator[](int idx) { return fields[idx];}
            //!< @brief grant read write random access to columns
            const std::string& operator[](int idx) const{ return fields[idx];}
            //!< @brief assign row_t object to another
            const row_t& operator=(const row_t& o) {fields = o.fields;return *this;}
            /**
            * @brief this assumes that the string object holds a csv line. It will be split into columns
            * @param [in] s: csv line
            * return reference the this
            */
            const row_t& operator=(const std::string& s)
            {*this=split(ignore_comment(s,__comment_starter),__delimiter);;return *this;}
            /**
            * @brief this assumes that the string object holds a csv line. It will be split into columns
            * @param [in] s: csv line
            * return reference the this
            */
            const row_t& operator=(const char* s)
            {*this=split(ignore_comment(s,__comment_starter),__delimiter);return *this;}

        private:
            const row_t& operator=(const std::vector<std::string>& o) {fields = o; return *this;}
    };

    int min_useful_columns;              //!< minimum columns to consider. Any line having less than this threshold are ignored.
    int lineno;                          //!< current line number


    /** \brief default construct
     * \param [in] mc: minimal useful columns
     */
    csv_parser_t(int mc =0):
          min_useful_columns(mc)
        , lineno (0)
        {
        }

    /** \brief iterates over file lines, and call user function back
     * the user callback function receives two parameters:
     *     - the current row
     *     - the current line number in input file
     * Then, it has to do its own process
     * \param [in] fn: filename
     * \param [in] callback: user callback
     * \return number of valid csv lines
     * \example each_row(std::cin, [](const row_t&, int lineno){do something})
     */
    int each_row(const std::string&fn,const std::function<void(const row_t&,int)>& callback)
    {
        std::ifstream ifs(fn);
        lineno = 0;
        int parsed = each_row(ifs,callback);
        ifs.close();
        return parsed;
    }
private:
    /** \brief iterates over stream lines, and call user function back
     * the user callback function receives two parameters:
     *     - the current row
     *     - the current line number in input file
     * Then, it has to do its own process
     * \param [in] in: input stream
     * \param [in] callback: user callback
     * \return number of valid csv lines
     * \example each_row(std::cin, [](const row_t&, int lineno){do something})
     */
    int each_row(std::istream& in,const std::function<void(const row_t&,int)>& callback)
    {
        int parsed=0;
        while(in.good())
            parsed+=next(in,callback);
        return parsed;
    }

    /** \brief parses next line, and call user function back if valid
     * the user callback function receives two parameters:
     *     - the current row
     *     - the current line number in input file
     * Then, it has to do its own process
     * \param [in] in: input stream
     * \param [in] callback: user callback
     * \return number 1 if the csv line is valid, 0 otherwise
     * \example each_row("some_file.csv", [](const row_t&, int lineno){do something})
     */

    int next(std::istream&in, const std::function<void(const row_t&,int)>& callback)
    {
        if(in.good())
        {
            char line[3*1024];
            in.getline(line,sizeof(line));
            lineno++;
            row_t current ;
            current =line;
            //current = line;
            if(current.size() < min_useful_columns)
                return 0;
            if(callback)
                callback(current,lineno);
            return 1;
        }
        return 0;
    }

};

typedef csv_parser_t<';','#'> tranche_file_parser_t;    //!< tranche file parser
typedef csv_parser_t<';','#'> format_file_parser_t;     //!< format file parser
typedef csv_parser_t<';','#'> labels_file_parser_t;     //!< labels file parser
typedef csv_parser_t<';','#'> generic_csv_parser_t;     //!< labels file parser
/**
* @} // addtogroup
*/

} /*namespace mparsers*/

#endif // CSV_UTILS_HEADER
