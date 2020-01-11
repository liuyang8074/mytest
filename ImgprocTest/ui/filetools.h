#pragma once
#include <vector>
#include <io.h>
#include <fstream>
#include <time.h>

class FileTools
{
public:


	static std::vector<std::string> getFiles(std::string path)
	{
		std::vector<std::string> files;
		getFiles(path, files);
		return files;
	}

	static void write_log(std::string filename, std::string log)
	{
		std::string savepath;
		std::string str = "D:";
		savepath = str+"\\"+filename;
		std::ofstream ofs(savepath, std::ios::app);

		time_t timp;
		struct tm* p;
		time(&timp);
		p=localtime(&timp);
        ofs << p->tm_year+1900 << "/" << p->tm_mon+1 << "/" << p->tm_mday << " " << p->tm_hour << ":" << p->tm_min << ":" << p->tm_sec << "   "<<log << std::endl;
	}

private:
	static void getFiles(std::string path, std::vector<std::string>& files)
	{
		//文件句柄  
		long   hFile = 0;
		//文件信息  
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo))!=-1)
		{
			do
			{
				//如果是目录,迭代之  
				//如果不是,加入列表  
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
				else
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

};
