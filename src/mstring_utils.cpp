#include <sstream>
#include <cstring>
#include <cstdio>
#include <parsers/mstring_utils.h>

namespace mparsers
{

    std::string build_filename(const std::string& str)
    {
        std::string result = str;
        for(int it=0; it < (int)result.length(); ++it)
        {
            if(!isalnum(result[it]))
                result[it]='_';
        }
        return result;
    }
    static
    std::vector<std::string> &split(const std::string &str, char delimiter, std::vector<std::string> &items)
    {
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delimiter))
        {
            items.push_back(item);
        }
        return items;
    }

    std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> items;
        split(str, delimiter, items);
        return items;
    }

    std::string ignore_comment(const std::string& str, char comment_char)
    {

        auto v=split(str,comment_char);
        if(v.empty())
            return "";
        return v[0];
    }

    static bool is_blank(int c)
    {
        if(c == ' ' || c=='\t' || c=='\r' || c == '\n')
            return true;
        return false;
    }

    char* strtrim(char* s)
    {
        if(!s)  return nullptr;
        char* res=s;

        for(;*res;++res)
            if(!is_blank(*res))
                break;

        int l = strlen(res);
        char* ptr = res + l - 1;
        for(;*ptr && ptr >= res && is_blank(*ptr);--ptr)
            *ptr=0;
        return res;
    }

    std::string stdstrim(const std::string& str)
    {
        char inbuf[1024];
        snprintf(inbuf,sizeof(inbuf),"%s",str.c_str());
        return std::string(strtrim(inbuf));
    }

    std::string splitpath(const std::string& filename)
    {
        auto last1 = filename.find_last_of('/');
        auto last2 = filename.find_last_of('\\');
        size_t last;
        if(last1 == std::string::npos && last2 == std::string::npos)
            return "";

        if(last1 == std::string::npos)
            last = last2;
        else
        {
            if(last2 == std::string::npos)
                last = last1;
            else
                last = std::max(last1,last2);
        }
        if(last == filename.size()-1)
            return filename;

        return filename.substr(0,last);
    }

    std::string splitext(const std::string& filename)
    {
        auto v = split(filename,'.');
        if(v.size())
            return v[v.size()-1];
        return "";
    }
} /*namespace mparsers*/

