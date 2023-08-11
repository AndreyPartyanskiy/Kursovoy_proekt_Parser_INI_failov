// Kursovoy_proekt_Parser_INI_failov.cpp 

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <any>

class ini_parser
{
public:
    std::fstream ptr_file_;
    std::string filename_;
    std::any my_any;

    ini_parser(std::string in_filename_) : filename_(in_filename_), ptr_file_(in_filename_)
    {
        std::cout << "file is open " << std::endl;
    }

    template <class T>
    T get_value(std::string in_data_)
    {
        std::string in_section_name_;
        std::string in_var_name_;
        
        int point = 0;
        for (char s : in_data_)
        {
            if (s == '.')point = 1;

            if (s!='.')
            if (!point ) in_section_name_ += s; else in_var_name_ += s;
        }

        std::string section_name_="";
        std::string var_name_="";
        std::string data_="";
        std::string comment="";

        int triger_section_ = 0;
        int triger_var_ = 0;
        int triger_data_ = 0;
        int triger_comment = 0;

        int true_section_ = 0;
        int true_var_ = 0;

        std::string out_data="";
          
        char ch;
        char ch_temp='\n';
        
        if (ptr_file_.is_open())
        {            
            while (!ptr_file_.eof())
            {
                ptr_file_.get(ch) ;
                if (ch=='\n'&&ch_temp =='\n') ptr_file_.get(ch);
                

                if (ch == '[' && triger_section_ == 0) triger_section_ = 1;
                if (ch == ';' && triger_comment ==0 ) triger_comment = 1;
                if (ch != '[' && ch != ';' && triger_var_ == 0 && ch_temp == '\n') triger_var_ = 1;                
                if (ch_temp == '=' && triger_data_ == 0) triger_data_ = 1;


                if (triger_data_ == 1 && ch != '=')
                {
                    if ( ch != '\n' && ch!=';')data_+= ch;
                    else
                    {
                        if (true_section_ == 1 && true_var_ == 1) out_data = data_;
                        triger_data_ = 0;
                        //std::cout  << data_ << std::endl;
                        std::cout << "\t \t \t" << out_data << true_section_ << "   " << true_var_ << std::endl;
                        data_ = "";

                    }
                }

                if(triger_var_ == 1 && triger_comment ==0 && triger_section_ == 0 )
                {
                    if (ch != '=')var_name_ += ch;
                    else
                    {
                        //std::cout << in_var_name_ << "" << var_name_ << std::endl;
                        if (var_name_ == in_var_name_)
                        { 
                            true_section_ = 1;

                        }
                        else true_section_ = 0;
                        
                        triger_var_ = 0;
                        var_name_ = "";

                    }
                }

                if(ch!='[')
                if (triger_section_ == 1)
                {
                    if (ch != ']' && ch!='\n')section_name_ += ch;
                    else
                    {
                        if (section_name_ == in_section_name_)
                      
                        {
                            true_var_ = 1;
                            //std::cout  << section_name_ << "" << in_section_name_ << std::endl;
                        }
                        else true_var_ = 0;

                        triger_section_ = 0;
                        section_name_ = "";

                    }
                }
                
                if (triger_comment == 1)
                {
                    if (ch != '\n')
                    {
                        comment += ch;
                    }
                    else
                    {
                        triger_comment = 0;
                        comment = "";
                    }
                }
                ch_temp = ch;
                
            }
            
        }
        
        if (typeid(T) == typeid(std::string))
        {
            std::cout << "string +++++++++" << std::endl;
            //std::cout << out_data << std::endl;
            my_any = std::string (out_data);
        }
        if (typeid(T) == typeid(int))
        {
           std::cout << "int +++++++++" << std::endl;
           //std::cout << out_data << std::endl;
           my_any = std::stoi(out_data);
           
        }
       
         return std::any_cast<T>(my_any);
    }

    ~ini_parser()
    {
        ptr_file_.close();
        std::cout << "file is close " << std::endl;
    }
};

int main()
{
    system("chcp 1251");
    system("cls");
    ini_parser parser("in_file.ini");
    auto value = parser.get_value<int>("Section1.var1");
    std::cout << "data = " << value <<std::endl;
    ini_parser parser1("in_file.ini");
    auto value_ = parser1.get_value<std::string>("Section1.var1");
    std::cout << "data = " << value_ << std::endl;
    return 0;
}
