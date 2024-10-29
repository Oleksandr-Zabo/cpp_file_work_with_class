#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>//getline, string
using namespace std;

class FileProcessResult abstract
{
public:
    virtual void get_message() = 0;
};

template<typename T>
class FileInterface abstract
{
public:
    virtual FileProcessResult* open(ios_base::openmode mode) = 0;
    virtual FileProcessResult* close() = 0;
    virtual FileProcessResult* write(T data, ios_base::openmode mode) = 0;
    virtual FileProcessResult* read(T& data, ios_base::openmode mode) = 0;
    virtual FileProcessResult* read_by_index(T& data, ios_base::openmode mode, int itemIndex = 0) = 0;
};

class FileOpenSuccess : public FileProcessResult
{
public:
    FileOpenSuccess() = default;
    void get_message() override
    {
        cout << "File opened successfully" << endl;
    }
};

class FileOpenError : public FileProcessResult
{
public:
    FileOpenError() = default;
    void get_message() override
    {
        cout << "Error opening file" << endl;
    }
};

class FileCloseSuccess : public FileProcessResult
{
public:
    FileCloseSuccess() = default;
    void get_message() override
    {
        cout << "File closed successfully" << endl;
    }
};

class FileWriteSuccess : public FileProcessResult
{
public:
    FileWriteSuccess() = default;
    void get_message() override
    {
        cout << "Data written successfully" << endl;
    }
};

class FileWriteError : public FileProcessResult
{
    exception e;
public:
    FileWriteError() = default;
    FileWriteError(exception e)
    {
        this->e = e;
    }
    void get_message() override
    {
        cout << "Error writing data" << endl;
    }
};

class FileReadSuccess : public FileProcessResult
{
public:
    FileReadSuccess() = default;
    void get_message() override
    {
        cout << "Data read successfully" << endl;
    }
};

class FileReadError : public FileProcessResult
{
    exception e;
public:
    FileReadError() = default;
    FileReadError(exception e)
    {
        this->e = e;
    }
    void get_message() override
    {
        cout << "Error writing data" << endl;
    }
};

template<typename T>
class FileService : public FileInterface<T>
{
    string filename;
    fstream file;
public:
    FileService(string filename)
    {
        this->filename = filename;
    }
    FileProcessResult* open(ios_base::openmode mode) override
    {
        FileProcessResult* result = nullptr;
        file.open(filename, mode);
        if (file.is_open())
        {
            result = new FileOpenSuccess();
        }
        else
        {
            result = new FileOpenError();
        }
        return result;
    }
    FileProcessResult* close() override
    {
        file.close();
        return new FileCloseSuccess();
    }
    FileProcessResult* write(T data, ios_base::openmode mode) override
    {
        try
        {
            FileProcessResult* result = this->open(mode);
            result->get_message();
            if (typeid(*result) == typeid(FileOpenSuccess))
            {
                if (mode == ios::app || mode == ios::out || mode == (ios::out | ios::binary) || mode == (ios::app | ios::binary))
                {
                    file.write((char*)&data, sizeof(T));
                }
                else
                {
                    return new FileWriteError();
                }
                result = this->close();
                result->get_message();
                return new FileWriteSuccess();
            }
            else if (typeid(*result) == typeid(FileOpenError))
            {
                return new FileWriteError();
            }
        }
        catch (exception e)
        {
            return new FileWriteError(e);
        }
    }

    FileProcessResult* read(T& data, ios_base::openmode mode) override
    {
        try
        {
            FileProcessResult* result = this->open(mode);
            result->get_message();
            if (typeid(*result) == typeid(FileOpenSuccess))
            {
                if (mode == ios::in || mode == (ios::in | ios::binary))
                {
                    file.read((char*)&data, sizeof(T));
                }
                else
                {
                    return new FileWriteError();
                }
                result = this->close();
                result->get_message();
                return new FileReadSuccess();
            }
            else if (typeid(*result) == typeid(FileOpenError))
            {
                return new FileReadError();
            }
        }
        catch (exception e)
        {
            return new FileReadError(e);
        }
    }

    FileProcessResult* read_by_index(T& data, ios_base::openmode mode, int itemIndex = 0) override
    {
        try
        {
            FileProcessResult* result = this->open(mode);
            result->get_message();
            if (typeid(*result) == typeid(FileOpenSuccess))
            {
                if (mode == ios::in || mode == (ios::in | ios::binary))
                {
                    // Move the file pointer to the position of the desired item
                    file.seekg(itemIndex * sizeof(T), ios::beg);//is used to move the file pointer to a specific position in the file
                    file.read((char*)&data, sizeof(T));
                }
                else
                {
                    return new FileWriteError();
                }
                result = this->close();
                result->get_message();
                return new FileReadSuccess();
            }
            else if (typeid(*result) == typeid(FileOpenError))
            {
                return new FileReadError();
            }
        }
        catch (exception e)
        {
            return new FileReadError(e);
        }
    }


    
};



//for using work with file
namespace FRACTION {
    class Fraction
    {
    private:
        int num, den;

    public:
        Fraction() {
            num = 0;
            den = 1;
        }
        Fraction(int num) :Fraction() {
            this->num = num;
        }
        Fraction(int num, int den) {
            this->num = num;
            this->den = den;
        }

        //setters

        void setNum(int num) {
            this->num = num;
        }
        void setDen(int den) {
            this->den = den;
        }

        //getters
        int getNum() {
            return num;
        }
        int getDen() {
            return den;
        }

        //methods

        void print() {
            if (num > den) {//if the numerator is greater than the denominator
                int whole = num / den;
                num = num % den;
                cout << whole << " " << num << "/" << den << endl;
            }
            else {
                cout << num << "/" << den << endl;
            }
            cout << endl;
        }

        void print(int numerator, int denominator) {
            if (numerator > denominator) {//if the numerator is greater than the denominator
                int whole = numerator / denominator;
                numerator = numerator % denominator;
                cout << whole << " " << numerator << "/" << denominator << endl;
            }
            else {
                cout << num << "/" << den << endl;
            }
            cout << endl;
        }

        void print(Fraction f) {
            if (f.num > f.den) {//if the numerator is greater than the denominator
                int whole = f.num / f.den;
                f.num = f.num % f.den;
                cout << whole << " " << f.num << "/" << f.den << endl;
            }
            else {
                cout << f.num << "/" << f.den << endl;
            }
            cout << endl;
        }

        Fraction add(Fraction f) {
            Fraction result;
            result.num = num * f.den + f.num * den;
            result.den = den * f.den;
            return result;
        }

        void print_add(Fraction f) {
            Fraction result;
            result.num = num * f.den + f.num * den;
            result.den = den * f.den;
            cout << "Addition: ";
            cout << "(" << num << "/" << den << ") + (" << f.num << "/" << f.den << ") = ";
            print(result);
        }

        Fraction sub(Fraction f) {
            Fraction result;
            result.num = num * f.den - f.num * den;
            result.den = den * f.den;
            return result;
        }

        void print_sub(Fraction f) {
            Fraction result;
            result.num = num * f.den - f.num * den;
            result.den = den * f.den;
            cout << "Subtraction: ";
            cout << "(" << num << "/" << den << ") - (" << f.num << "/" << f.den << ") = ";
            print(result);
        }

        Fraction mul(Fraction f) {
            Fraction result;
            result.num = num * f.num;
            result.den = den * f.den;
            return result;
        }
        void print_mul(Fraction f) {
            Fraction result;
            result.num = num * f.num;
            result.den = den * f.den;
            cout << "Multiplication: ";
            cout << "(" << num << "/" << den << ") * (" << f.num << "/" << f.den << ") = ";
            print(result);
        }

        Fraction div(Fraction f) {
            Fraction result;
            result.num = num * f.den;
            result.den = den * f.num;
            return result;
        }
        void print_div(Fraction f) {
            Fraction result;
            result.num = num * f.den;
            result.den = den * f.num;
            cout << "Division: ";
            cout << "(" << num << "/" << den << ") / (" << f.num << "/" << f.den << ") = ";
            print(result);
        }


    };
}



int main() {

    try {
        //create obj
        FRACTION::Fraction f1(3, 4);
        f1.print();
        cout << endl;

        //create file
        FileService<FRACTION::Fraction> file("data.bin");
        FileProcessResult* result = nullptr;

        //write data to file
        result = file.write(f1, ios::out | ios::binary);
        result->get_message();

        //create obj
        FRACTION::Fraction f2(2, 3);
        f2.print();
        cout << endl;

        //append data to file
        result = file.write(f2, ios::app | ios::binary);
        result->get_message();

        //read data(only 1-st line)
        FRACTION::Fraction new_fraction;
        result = file.read(new_fraction, ios::in | ios::binary);

        //read data for 2-nd line
        int index_item = 1;//li
        FRACTION::Fraction new_fraction_1;
        result = file.read_by_index(new_fraction_1, ios::in | ios::binary, index_item);
        
        //write to console data from file
        cout << "Data from file(1-st item):" << endl;
        new_fraction.print();

        cout << "Data from file(2-nd item):" << endl;
        new_fraction_1.print();

        result->get_message();
    }
    catch (FileProcessResult* file_error)
    {
        file_error->get_message();
    }

    system("pause");
    return 0;
}