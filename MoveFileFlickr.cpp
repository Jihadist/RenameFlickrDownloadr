// MoveFileFlickr.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "Windows.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>


int main(int argc,char **argv)
{
	// You need provide only correct path 
	// указываем путь до директории
	std::wstring path ={L"D:\\Media\\Flickr\\flickr-downloadr-2019-01-05_09-25-35\\My set\\"};
	using nlohmann::json;
	WIN32_FIND_DATAW wfd;
	// цикл поиска файлов с расширением json, т.е. содержащих название исходной фотографии
	HANDLE const hFind = FindFirstFileW((path+L"*.json").c_str(), &wfd);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			// имя файла .json
			auto file = &wfd.cFileName[0];
			// имя файла уже без расширения
			std::wstring pictName= std::wstring(file).substr(0, int(std::wstring(file).find_first_of(L".")));
			// выведем его для проверки
			std::wcout << pictName << std::endl;
			// парсим json
			std::ifstream i(path + std::wstring(file));
			json j = json::parse(i);
			// находим название
			auto title = j.at("Title");
			// получаем исходное название фотографии в json формате
			std::string temp_name = title.begin().value().dump();
			// переводим в исходный вид
			std::string fileName = temp_name.substr(1, temp_name.size() - 2) + ".jpg";
			// выведем его для проверки
			std::wcout << file<< " : " << 
				fileName.c_str() << std::endl;
			// далее запускаем процесс переименования
			// первый аргумент это имя директории + имя исходное имя файла + расширение
			if(MoveFile(std::wstring(path+pictName+L".jpg").c_str(),
				// второй аргумент это имя директории + уже готовое имя файла с расширением
				(path+std::wstring(fileName.begin(),fileName.end())).c_str()))
				// если удачно то выведем имя файла конечное
				std::wcout<<"renamed:"<< std::wstring(path + pictName + L".jpg").c_str()<<std::endl;

		} while (NULL != FindNextFileW(hFind, &wfd));

		FindClose(hFind);
	}
    std::cout << "Ending\n"; 
}