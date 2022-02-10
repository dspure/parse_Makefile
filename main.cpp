#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

//compare with v1 and v2, remove the v2 option if v1 not have
void do_compare(std::vector<std::string> &mak, std::vector<std::string> &def)
{
	for(auto &s : def)
	{
		for(auto &s2 : mak)
		{
			if(s2.find(s, 0) != std::string::npos)
				s2 += "<===== HAVE THE CONFIG OPTION";
		}
	}
}

bool find_equal(char *buf)
{
	int len = strlen(buf);
	for(int i = 0; i < len; i++)
	{
		if(buf[i] == '=')
			return true;
	}
	return false;
}

//remove char below c inside buf
bool getsubstr(char *buf, char c)
{
	int len = strlen(buf);
	for(int i = 0; i < len; i++)
	{
		if(buf[i] == c)
		{
			buf[i] = '\0';
			return true;
		}
	}
	return false;

}

int main(int argc, char *argv[])
{
	// -m Makefile
	// -d defconfig file
	// -o out put no invailed option makefile
	// --help/-h out put help info
	if (argc == 1)
		std::cout << "parse_make_file version 0.1" << std::endl
			<< "invailed parameter" << std::endl
			<< "    you can also use \"--help\"" << std::endl;
	int i = 0;
	std::fstream makefile;
	std::ifstream defconfig;
	std::ofstream outfile;
	std::string mkpath;

	bool direction = false; // direction modify Makefile flag
	bool showmk = false; // show makefile after modify flag
	bool showdf = false; // show defconfig after modify flag

	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "-D") == 0)
				direction = true;
	}


	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "-m") == 0)
		{
			makefile.open(argv[i+1], std::ios_base::in);
			mkpath = argv[i+1];
		}
		if(strcmp(argv[i], "-d") == 0)
		{
			defconfig.open(argv[i+1], std::ios_base::in);
		}
		if(strcmp(argv[i], "-o") == 0)
		{
			outfile.open(argv[i+1], std::ios_base::out);
		}
		if(strcmp(argv[i], "-s") == 0)
		{
			showmk = true;
		}
		if(strcmp(argv[i], "-S") == 0)
		{
			showdf = true;
		}
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			std::cout << "parse_make_file version 0.1" << std::endl << std::endl
				<< "-m Makefile" << std::endl
			        << "-d defconfig file" << std::endl
				<< "-D direction modify Makefile" << std::endl
				<< "-o out put no invailed option makefile" << std::endl
				<< "-s show makefile context after parse" << std::endl
				<< "-S show defconfig context after parse" << std::endl
				<< "--help/-h out put help info" << std::endl;
		}
		if(strcmp(argv[i], "--version") == 0)
			std::cout << "parse_make_file version 0.1" << std::endl;
	}

	/*
	if(is_ok < 2)
	{
		makefile.close();
		defconfig.close();
		outfile.close();
		return -2;
	}*/
	char buf[1024];
	std::vector<std::string> mak_vec;
	std::vector<std::string> def_vec;

	//parse makefile to mak_vec
	if(makefile.is_open())
	{
		memset(buf, 0, sizeof(buf));
		while(!makefile.eof())
		{
			makefile.getline(buf,sizeof(buf));
			mak_vec.push_back(std::string(buf));
		}
	}

	//parse defconfig to def_vec
	if(defconfig.is_open())
	{
		memset(buf, 0, sizeof(buf));
		while(!defconfig.eof())
		{
			defconfig.getline(buf, sizeof(buf));
			if(find_equal(buf) && buf[0] != '#')
			{
				getsubstr(buf, '=');
				def_vec.push_back(std::string(buf));
			}
		}
	}

	//compare two file
	if(makefile.is_open() && defconfig.is_open())
	{
		do_compare(mak_vec, def_vec);
	}
	
	//write result to makefile direction  
	if(direction)
	{
		if(makefile.is_open())
		{
			makefile.close();
		}
		makefile.open(mkpath.c_str(), std::ios_base::out);
		if(makefile.is_open())
		{
			for(auto s : mak_vec)
			{
				s += "\n";
				makefile.write(s.c_str(), s.length());
			}
		}
	}

	// write result to outfile
	if(outfile.is_open())
	{	
		for(auto s : mak_vec)
		{
			s += "\n";
			outfile.write(s.c_str(), s.length());
		}
		outfile.close();
	}

	//show makefile and defconfig
	if(showmk)
	{
		for(auto &s : mak_vec)
			std::cout << s.c_str() << std::endl;
	}

	if(showdf)
	{
		for(auto &s : def_vec)
			std::cout << s.c_str() << std::endl;
	}

	//close all file stream
	{
		makefile.close();
		defconfig.close();
		outfile.close();
	}
	return 0;
}



