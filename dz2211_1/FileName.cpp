#include<iostream>
#include<thread>
#include<mutex>
#include<fstream>
#include<ctime>
#include<vector>
#include<windows.h>

const std::string FILE_NAME = "digits.txt";  

bool done = false; 

std::vector<int> numbers;  

std::mutex fileMutex;  

void NumToFile()
{
    fileMutex.lock();  
    srand(time(NULL));
    std::ofstream file(FILE_NAME);
    for (int i = 0; i < 20; i++)
    {
        file << rand() % 100 << std::endl;
    }
    fileMutex.unlock(); 
}

void NumFromFile()
{
    fileMutex.lock();
    int num;
    std::ifstream file(FILE_NAME);
    while (file >> num)  
    {
        numbers.push_back(num);
    }
    fileMutex.unlock(); 
}

void RemoveEven()
{
    if (numbers.empty()) return;  
    for (auto it = numbers.begin(); it != numbers.end();)
    {
        if (*it % 2 == 0)  
        {
            it = numbers.erase(it);  
        }
        else
        {
            ++it;  
        }
    }
}

void RemoveOdd()
{
    if (numbers.empty()) return;  
    for (auto it = numbers.begin(); it != numbers.end();)
    {
        if (*it % 3 == 0)  
        {
            it = numbers.erase(it);  
        }
        else
        {
            ++it; 
        }
    }
}

void PrintNumbers()
{
    if (numbers.empty())
    {
        std::cout << "No numbers available." << std::endl;
    }
    else
    {
        for (size_t i = 0; i < numbers.size(); i++)
        {
            std::cout << "Element [" << i << "]: " << numbers[i] << std::endl;
        }
    }
    done = true;  
}

int main()
{
    NumFromFile();
    std::thread removeEvenThread(RemoveEven); 
    std::thread removeOddThread(RemoveOdd);  
    std::thread printThread(PrintNumbers);  

    removeEvenThread.join();
    removeOddThread.join();
    printThread.join();

    while (!done) 
    {
        Sleep(1); 
    }

    return 0;
}
