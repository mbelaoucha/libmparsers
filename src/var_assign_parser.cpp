#include <fstream>
#include <cstring>
#include <parsers/mstring_utils.h>
#include <parsers/var_assign_parser.h>

using namespace mparsers;
void opt_parser_t::add_cmd_parser(const std::string& cmd, void* ud
                        ,const std::function<bool(const std::string& cb, char*,void*,int)>&cb)
{
    opt_parser_t::command_parser_t cp;
    cp.command = cmd;
    cp.callback = cb;
    cp.user_data = ud;
    commands.push_back(cp);
}

void opt_parser_t::set_unexpected_cmd_handler(void*ud
                , const std::function<bool(const std::string&,char*,void*,int)>& cb)
{
    unknown.command="dummy";
    unknown.user_data=ud;
    unknown.callback = cb;
}

bool opt_parser_t::__user_value_parser(const std::string& _command,char* value,bool& interrupt)
{
    for(auto it=commands.begin(); it != commands.end(); ++it)
    {
        if( it->command != _command)
            continue;

        if(it->callback)
            interrupt=it->callback(_command, value, it->user_data,lineno);
        return true;
    }

    if(unknown.callback)
        interrupt = unknown.callback(_command,value, unknown.user_data,lineno);
    return false;
}

bool opt_parser_t::parse(std::istream& streamin)
{
    char curent_line[1024];
    int errors = 0;
    lineno = 0;
    while(streamin.good())
    {
        lineno++;
        streamin.getline(curent_line,sizeof(curent_line));
        bool interrupt=false;
        errors += parse_line(curent_line,interrupt);
        if(interrupt)
            return false;
    }
    return true;
}

bool opt_parser_t::parse(const std::string& fn)
{
    std::ifstream ifs;
    ifs.open(fn,std::ifstream::in);
    bool result=false;
    if(ifs.good())
        result = parse(ifs);

    ifs.close();
    return result;
}

bool opt_parser_t::parse_line(char* line, bool& interrupt)
{
    if(!line)
        return false;
    if(*line == '#' || *line ==';')
        return false;

    char* useful = strtok(line,"#;");
    if(!useful)
        return false;

    char* command_untrimed = strtok(useful,"=");
    if(! command_untrimed)
        return false;
    char* _command;


    _command = strtrim(command_untrimed);
    if(!_command || !*_command)
        return false;

    char* value = strtok(nullptr,"#;");

    __user_value_parser(_command,value, interrupt);
    return true;
}

