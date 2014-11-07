#ifndef VAR_ASSIGN_PARSER_T_HEADER_INC
#define VAR_ASSIGN_PARSER_T_HEADER_INC

#include <iostream>
#include <functional>
#include <vector>

#include <cassert>


#ifndef GO_UNREACHABLE
#define GO_UNREACHABLE() assert(0)
#endif

namespace mparsers
{

/**
 * \defgroup parsers_group Parsers
 * \brief some useful parsers
 * @{
 * \file var_assign_parser.h
 * \brief parser for variable assignment sequence file.
 * \author Marouane BELAOUCHA
 */


/**
 *  \brief option file parser

## How To Use? ##
 The file to parse is a sequence of variable assignment commands of this form:
~~~~~~~~~~
1:    # this is a comment
2:    COMMAND_ONE = this is value  ; this will be ignored
3:    COMMAND_two = ETH 1 0 20 15    # this will be ignored
4:    cOmMaNd_tHrEe = dummy
    ...
~~~~~~~~~~
Here characters ';' and '#' are used as comment starters, with effect till the end of line.
The #opt_parser_t parser iterates over file's lines looking for assignments. The assignment
left hand side denotes the command name (case sensitive) which should be registered using
opt_parser_t#add_cmd_parser, before paring with  opt_parser_t#parse.

The input seen by parser for the example above is:
~~~~~~~~~~
1:
2:    COMMAND_ONE = this is value
3:    COMMAND_two = ETH 1 0 20 15
4:    cOmMaNd_tHrEe = dummy
    ...
~~~~~~~~~~

A parser for such file could be initialized this way:

~~~~~~~~~~
opt_parser_t parser;
...
parser.add_cmd_parser("COMMAND_ONE", nullptr, [](const std::string& cmd, char* value, void* userdata, int line)
                      {
                        assert(cmd == "COMMAND_ONE");
                        assert(userdata == nullptr);
                        cout <<std::endl<< "line#"<<line<<": the command  '"<<cmd<<"' got the value '"<<value<<"'";
                      });

parser.add_cmd_parser("COMMAND_two", nullptr, [](const std::string& cmd, char* value, void* userdata, int line)
                      {
                        assert(cmd == "COMMAND_two");
                        assert(userdata == nullptr);
                        cout <<std::endl<< "line#"<<line<<": the command  '"<<cmd<<"' got the value '"<<value<<"'";
                      });

parser.set_unexpected_cmd_handler(nullptr,[](const std::string& cmd, char* value, void* userdata, int line)
                    {
                        cout <<std::endl<<"line#"<<line<<": unhandled command  '"<<cmd<<"' got the value '"<<value<<"'";
                    });

parser.parse("my_config_file.cfg");
~~~~~~~~~~

The output should look like:
~~~~~~~~~~
line#2: the command 'COMMAND_ONE' got the value ' this is value  '
line#3: the command 'COMMAND_two' got the value 'ETH 1 0 20 15    '
line#4: unhandled command  'cOmMaNd_tHrEe' got the value ' dummy'
~~~~~~~~~~

## Commands Provider ##
the opt_parser_t#init method is designed to register commands parsers for a particular use.
This last should provide a static method #register_command which does the commands registering.
class #config_parser_filler_t is a good example on how this collaboration is done.
\see config_parser_filler_t#register_commands, config_t#load
*/
struct opt_parser_t
{
    /**
    * \brief One single command parser
    */
    struct command_parser_t
    {
        std::string command;                                                    //!< concerned command
        std::function<bool(const std::string&, char*, void*,int)> callback;     //!< user defined callback
        void* user_data;                                                        //!< user defined data

        command_parser_t() :
            command("dummy")
            , callback(nullptr)
            , user_data(nullptr)
            {}

        command_parser_t(const command_parser_t& other)
            :
              command(other.command)
            , callback(other.callback)
            , user_data(other.user_data)
        {}

        const command_parser_t& operator=(const command_parser_t& rhs)
        {
            command=rhs.command;
            callback=rhs.callback;
            user_data=rhs.user_data;
            return *this;
        }

        ~command_parser_t(){}
    };

    std::vector<command_parser_t>   commands;           //!< commands parser
    command_parser_t                unknown;            //!< callback this for any unknown command
    int                             lineno;             //!< current line number in parsed file
    void*                           user_data;          //!< global user data


    opt_parser_t():
          commands()
        , unknown()
        , lineno(0)
        , user_data(nullptr)
    {}

    ~opt_parser_t(){}


    template<class filler_t>
    void init(void* ud=nullptr)
    {
        user_data = ud;
        filler_t::register_commands(*this);
    }

    /**
    * \brief parse commands from the input stream
    * \param [in,out] input: input stream
    * \return useless
    */
    bool parse(std::istream& input);
    /**
    * \brief parse commands filename
    * \param [in] filename: input file name
    * \return useless
    */
    bool parse(const std::string& filename);

    /**
    * \brief register a new command, with user data, and a callback function
    * \param [in] cmd: command name
    * \param [in,out] userdata: user data to be passed to the callback function
    * \param [in] callback: function to call
    * the callback function receives four arguments:
    *    - cmd: the command (assignment's left-hand-side)
    *    - val: the value (assignment's right hand side)
    *    - userdata: the registered user data
    *    - line: line number in file
    */
    void add_cmd_parser(const std::string& cmd, void* userdata
                , const std::function<bool(const std::string& cmd,char* val,void* userdata,int line)>& callback);

    /**
    * \brief register a callback for unregistered commands
    * \param [in,out] userdata: user data to be passed to the callback function
    * \param [in] callback: function to call
    */
    void set_unexpected_cmd_handler(void* userdata, const std::function<bool(const std::string&,char*,void*,int)>& callback);

private:
    /**
    * \brief parse one single line
    * \param [in,out] line
    * \param [in,out] interrupt: unused
    */
    bool parse_line(char* line, bool& interrupt);

    /**
    * \brief call the callback corresponding to a command
    * \param [in] command: the current command
    * \param [in,out] value: the corresponding value
    * \param [in,out] interrupt: unused
    */
    bool __user_value_parser(const std::string& command,char* value, bool& interrupt);

    opt_parser_t(const opt_parser_t&) :
        commands()
        , unknown()
        , lineno(0)
        , user_data(nullptr)
    {GO_UNREACHABLE();}
    const opt_parser_t& operator=(const opt_parser_t&) {GO_UNREACHABLE();}

};

} /* namespace mparsers */
/**
 * @}
 */

#endif // VAR_ASSIGN_PARSER_T_HEADER_INC
