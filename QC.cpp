#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <aclapi.h>
#include <strsafe.h>
#include <wininet.h>
#include <stdexcept>
#include <locale>
#include <thread>
#include <mutex>
#include "QC.h"
#define CORES 4

mutex m1;

// IO functions.
void err(wstring func)
{
	DWORD num = GetLastError();
	LPWSTR buffer = new WCHAR[512];
	wstring mod = L"wininet.dll";
	LPCWSTR modul = mod.c_str();
	DWORD buffer_length = FormatMessageW((FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE), GetModuleHandleW(modul), num, 0, buffer, 512, NULL);
	DWORD a = GetLastError();
	wstring message(buffer, 512);
	delete[] buffer;
	wstring output = func + L" caused error " + to_wstring(num) + L": " + message;
	m1.lock();
	wcout << output << endl;
	m1.unlock();
	exit(EXIT_FAILURE);
}
void report(wstring func, wstring catalogue)
{
	wstring output = func + L" was fixed in " + catalogue;
	m1.lock();
	wcout << output << endl;
	m1.unlock();
}

// Read into memory a (local .bin file / webpage).
wstring bin_memory(HANDLE& hfile)
{
	DWORD size = GetFileSize(hfile, NULL);
	DWORD bytes_read;
	LPWSTR buffer = new WCHAR[size / 2];
	if (!ReadFile(hfile, buffer, size, &bytes_read, NULL))
	{
		err(L"ReadFile-bin_memory");
	}
	wstring bin(buffer, size / 2);
	delete[] buffer;
	return bin;
}

// Adjust the number of empty spaces in a specific place within a file in memory.
void respacer(wstring& page, size_t pos0, int desired_spaces)
{
	size_t pos1 = page.find_first_not_of(L' ', pos0 + 1);  // pos0 is the final position preceeding the spaces.
	int current_spaces = pos1 - pos0 - 1; 
	int delta_spaces = desired_spaces - current_spaces;
	if (delta_spaces < 0)
	{
		page.erase(pos0 + 1, abs(delta_spaces));
	}
	else if (delta_spaces > 0)
	{
		for (int ii = 0; ii < delta_spaces; ii++)
		{
			page.insert(pos0 + 1, L" ");
		}
	}
}

// Extremely specific CSV file fixes.
int fix1986a(wstring& csv_file)  // Income-indent fix.
{
	size_t pos1, pos2, pos3;
	pos1 = csv_file.find(L"years and over with income", 0);
	pos1 = csv_file.find(L'\n', pos1 + 1);
	pos1 = csv_file.find(L'\n', pos1 + 1);
	pos2 = csv_file.rfind(L'$', pos1 + 9);
	if (pos2 > pos1) // Apply the fix only if the old error is present!
	{
		csv_file.erase(pos2, 2);
		pos3 = csv_file.find(L'$', pos2);
		csv_file.erase(pos3, 2);
		pos1 = csv_file.find(L'\n', pos3);
		for (int kk = 0; kk < 3; kk++)
		{
			pos2 = csv_file.find(L' ', pos1);
			csv_file.erase(pos2, 2);
			pos3 = csv_file.find(L'-', pos2);
			csv_file.erase(pos3 + 1, 2);
			pos1 = csv_file.find(L'\n', pos3);
		}
		for (int kk = 0; kk < 5; kk++)
		{
			pos2 = csv_file.find(L' ', pos1);
			csv_file.erase(pos2, 1);
			pos3 = csv_file.find(L'-', pos2);
			csv_file.erase(pos3 + 1, 1);
			pos1 = csv_file.find(L'\n', pos3);
		}
		pos2 = csv_file.find(L' ', pos1);
		csv_file.erase(pos2, 1);

		pos3 = csv_file.find(L"years and over with income", pos2);
		pos1 = csv_file.find(L'\n', pos3);
		pos1 = csv_file.find(L'\n', pos1 + 1);
		pos2 = csv_file.find(L'$', pos1);
		csv_file.erase(pos2, 2);
		pos3 = csv_file.find(L'$', pos2);
		csv_file.erase(pos3, 2);
		pos1 = csv_file.find(L'\n', pos3);
		for (int kk = 0; kk < 3; kk++)
		{
			pos2 = csv_file.find(L' ', pos1);
			csv_file.erase(pos2, 2);
			pos3 = csv_file.find(L'-', pos2);
			csv_file.erase(pos3 + 1, 2);
			pos1 = csv_file.find(L'\n', pos3);
		}
		for (int kk = 0; kk < 5; kk++)
		{
			pos2 = csv_file.find(L' ', pos1);
			csv_file.erase(pos2, 1);
			pos3 = csv_file.find(L'-', pos2);
			csv_file.erase(pos3 + 1, 1);
			pos1 = csv_file.find(L'\n', pos3);
		}
		pos2 = csv_file.find(L' ', pos1);
		csv_file.erase(pos2, 1);

		pos3 = csv_file.find(L"Family income", pos2);
		pos1 = csv_file.find(L'\n', pos3);
		pos1 = csv_file.find(L'\n', pos1 + 1);
		pos2 = csv_file.find(L'$', pos1);
		csv_file.erase(pos2, 2);
		pos3 = csv_file.find(L'$', pos2);
		csv_file.erase(pos3, 2);
		pos1 = csv_file.find(L'\n', pos3);
		for (int kk = 0; kk < 7; kk++) 
		{
			pos2 = csv_file.find(L'"', pos1);
			respacer(csv_file, pos2, 2);
			pos3 = csv_file.find(L'-', pos2);
			respacer(csv_file, pos3, 1);
			pos1 = csv_file.find(L'\n', pos3);
		}
		pos2 = csv_file.find(L'"', pos1);
		respacer(csv_file, pos2, 2);

		pos3 = csv_file.find(L"Household income", pos2);
		pos1 = csv_file.find(L'\n', pos3);
		pos1 = csv_file.find(L'\n', pos1 + 1);
		pos2 = csv_file.find(L'$', pos1);
		csv_file.erase(pos2, 2);
		pos3 = csv_file.find(L'$', pos2);
		csv_file.erase(pos3, 2);
		pos1 = csv_file.find(L'\n', pos3);
		for (int kk = 0; kk < 7; kk++)
		{
			pos2 = csv_file.find(L'"', pos1);
			respacer(csv_file, pos2, 2);
			pos3 = csv_file.find(L'-', pos2);
			respacer(csv_file, pos3, 1);
			pos1 = csv_file.find(L'\n', pos3);
		}
		pos2 = csv_file.find(L'"', pos1);
		respacer(csv_file, pos2, 2);

		return 1;
	}
	return 0;
}

// Query necessary data from the user for subsequent operations.
int user_input(wstring& year, vector<wstring>& catalogues)
{
	wcout << L"Specify year to fix (";
	for (int ii = 0; ii < years.size(); ii++)
	{
		wcout << years[ii];
		if (ii < years.size() - 1) { wcout << L", "; }
	}
	wcout << L"), or type 'all' to fix all years." << endl;
	wcin >> year;

	int YEAR;
	if (year == L"all") { return 0; }
	else
	{
		try
		{
			YEAR = stoi(year);
		}
		catch (invalid_argument& ia)
		{
			wcout << L"Invalid value for the year. Press any key, then try again.";
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	WIN32_FIND_DATAW info;
	HANDLE hfile1 = INVALID_HANDLE_VALUE;
	wstring year_search, folder_path, catalogue_choice, temp1;
	vector<wstring> catalogue_paths;
	size_t pos1;

	year_search = local_directory + L"\\" + year + L"\\*";
	hfile1 = FindFirstFileW(year_search.c_str(), &info);
	do
	{
		folder_path = local_directory + L"\\" + year + L"\\" + info.cFileName;
		pos1 = folder_path.find(L'.', 0);
		if (pos1 < folder_path.size())
		{
			continue;
		}
		else
		{
			catalogue_paths.push_back(folder_path);
			catalogues.push_back(info.cFileName);
		}
	} while (FindNextFileW(hfile1, &info));
	if (!FindClose(hfile1)) 
	{ 
		wcout << L"Error closing hfile1 in user_input." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	wcout << endl << endl << L"Specify a Catalogue Number (from the options below) to fix, or type 'all' ";
	wcout << L"to fix all catalogues in " << year << L"." << endl << endl;

	int right = 0;
	for (int ii = 0; ii < catalogues.size(); ii++)
	{
		if (right > 3)
		{
			wcout << setw(20) << catalogues[ii] << endl;
			right = 0;
		}
		else
		{
			wcout << setw(20) << catalogues[ii];
			right++;
		}
	}
	wcout << endl << endl;
	wcin >> catalogue_choice;
	if (catalogue_choice == L"all") { return 0; }
	else
	{
		while (catalogue_choice.back() == L' ')
		{
			catalogue_choice.pop_back();
		}
		for (int ii = 0; ii < catalogue_paths.size(); ii++)
		{
			if (catalogue_choice == catalogues[ii])
			{
				temp1 = catalogues[ii];
				catalogues.clear();
				catalogues.push_back(temp1);
				return 0;
			}
		}
		wcout << L"Invalid value for the catalogue. Press any key, then try again.";
		system("pause");
		exit(EXIT_FAILURE);
	}
	return 1;
}

// Single thread's workload in correcting catalogue errors.
void fix_one(wstring file_path, wstring catalogue)
{
	HANDLE hfile1 = CreateFileW(file_path.c_str(), GENERIC_ALL, (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE), NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile1 == INVALID_HANDLE_VALUE) { err(L"CreateFile-fix_one"); }
	wstring csv_file = bin_memory(hfile1);

	bool written = 0;
	DWORD bytes;
	int work_done = 0;
	work_done = fix1986a(csv_file);
	
	if (work_done) 
	{
		if (!DeleteFileW(file_path.c_str())) { err(L"DeleteFile-fix_one"); }
		if (!CloseHandle(hfile1)) { err(L"CloseHandle-fix_one"); }
		hfile1 = CreateFileW(file_path.c_str(), GENERIC_ALL, (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE), NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hfile1 == INVALID_HANDLE_VALUE) { err(L"CreateFile2-fix_one"); }
		written = WriteFile(hfile1, csv_file.c_str(), csv_file.size() * 2, &bytes, NULL);
		if (!written) { err(L"WriteFile-fix_one"); }
	}
	if (!CloseHandle(hfile1)) { err(L"CloseHandle2-fix_one"); }
}

void fix(wstring& year, vector<wstring>& catalogues)
{
	// Let's assume, for all years, that currency is always Canadian dollars $ (CAD). 
	wstring year_folder;
	wstring catalogue_folder;
	wstring folder_search;
	WIN32_FIND_DATAW info;
	wstring file_name, csv_file;
	vector<wstring> file_paths;
	HANDLE hfile1 = INVALID_HANDLE_VALUE;
	int CSVs_checked = 0;
	vector<thread> tapestry;

	// All things 1986.
	if (year == L"1986" || year == L"all")
	{
		for (int ii = 0; ii < catalogues.size(); ii++)
		{
			// All things 97-570-X1986003.
			if (catalogues[ii] == L"97-570-X1986003")
			{
				catalogue_folder = local_directory + L"\\1986\\97-570-X1986003";
				folder_search = catalogue_folder + L"\\*.csv";
				hfile1 = FindFirstFileW(folder_search.c_str(), &info);
				if (hfile1 == INVALID_HANDLE_VALUE) { err(L"FindFirstFile-fix"); }
				do
				{
					file_name = catalogue_folder + L"\\" + info.cFileName;
					file_paths.push_back(file_name);
				} while (FindNextFileW(hfile1, &info));
				if (!FindClose(hfile1)) { err(L"FindClose-fix"); }

				while (CSVs_checked < file_paths.size())
				{
					for (int jj = 1; jj < CORES; jj++) // Leave 1 core unused for system stability.
					{
						if (CSVs_checked < file_paths.size())
						{
							thread threadx(fix_one, file_paths[CSVs_checked], L"97-570-X1986003");
							tapestry.push_back(move(threadx));
							CSVs_checked++;
						}
					}
					for (auto& th : tapestry)
					{
						if (th.joinable())
						{
							th.join();
						}
					}
				}
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, ".UTF8");
	wstring year;
	vector<wstring> catalogues;
	
	int error = user_input(year, catalogues);
	if (error > 0)
	{
		wcout << L"Error in user_input." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	fix(year, catalogues);

	system("pause");
	return 0;
}