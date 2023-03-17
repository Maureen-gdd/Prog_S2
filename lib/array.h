#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <stdexcept>
#include <time.h>
#include <chrono>
#include <thread>
#include <string>

#include <qglobal.h>

#include <QStringList>

#include <memory>
#include <string>
#include <stdexcept>

#include "mainwindow.h"
#include "utils.h"

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

class AbstractArray
{
public:
    AbstractArray(size_t size=100);
    AbstractArray(const AbstractArray& other);

    virtual ~AbstractArray() {}

    virtual void resize(size_t size);
    virtual void reserve(size_t size) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
    virtual size_t effectiveSize() const = 0;
    virtual QString toString() const = 0;

    virtual void swap(const int index1, const int index2) = 0;

    bool hasBeenReadenAt(const int index);
    bool hasBeenWrittenAt(const int index);
    int readAccessCount() const {return _readAccess;}
    int writeAccessCount() const {return _writeAccess;}

    static bool wait_for_operations;
    void operation_sleep(float factor=1.0f) const;
protected:
    std::vector<bool> _hasBeenReaden;
    std::vector<bool> _hasBeenWritten;
    int _readAccess;
    int _writeAccess;
};

template <typename T>
class TemplateArray : public AbstractArray
{
public:
    typedef	T ElementType;
    typedef AbstractArray Base;

    TemplateArray(size_t size=100)
        : AbstractArray(size)
    {
        resize(size);
    }

    TemplateArray(const TemplateArray<ElementType>& other)
        : AbstractArray(other), _data(other._data)
    {}
    TemplateArray(const std::vector<ElementType>& other)
        : AbstractArray(other.size()), _data(other.cbegin(), other.cend())
    {}

    virtual ~TemplateArray() {}

    virtual void clear(int, int) {}

    virtual void resize(size_t size)
    {
        Base::resize(size);
        int oldSize = _data.size();
        if (size>0)
        {
            _data.resize(size);
            clear(oldSize, size);
        }
    }

    virtual void reserve(size_t size)
    {
        _data.reserve(size);
    }

    virtual bool empty() const
    {
        return _data.empty();
    }

    void push_back(const ElementType& value);

    bool contains(const ElementType& value) const
    {
        for (ElementType d : _data)
            if (d==value)
                return true;
        return false;
    }

    size_t size() const
    {
        return _data.size();
    }

    ElementType& get(const int index)
    {
        if(index >= (int)_data.size())
            throw std::out_of_range(string_format("get(): Given index (%d) is out of bound !", index));
        _hasBeenReaden[index] = true;
        _readAccess++;
        operation_sleep();
        return _data[index];
    }

    ElementType get(const int index) const
    {
        TemplateArray<T>* self = const_cast<TemplateArray<T>*>(this);
        if(index >= (int)_data.size())
            throw std::out_of_range(string_format("get(): Given index (%d) is out of bound !", index));
        self->_hasBeenReaden[index] = true;
        self->_readAccess++;
        operation_sleep();
        return _data[index];
    }

    const ElementType& __get__(const int index) const
    {
        return _data[index];
    }

    ElementType& __get__(const int index)
    {
        return _data[index];
    }

    void set(const int index, const ElementType& value)
    {
        __set__(index, value);
    }

    void __set__(const int index, const ElementType& value)
    {
        if(index >= (int) _data.size())
            throw std::out_of_range(string_format("set(): Given index (%d) is out of bound !", index));
        _data[index] = value;
    }

    void swap(const int index1, const int index2)
    {
        if(index1 >= (int)_data.size() || index2 >= (int)_data.size())
            throw std::out_of_range(string_format("swap(): Given index (%d, %d) is out of bound !", index1, index2));
        const ElementType tmp = _data[index1];
        _data[index1] = _data[index2];
        _data[index2] = tmp;
        _hasBeenWritten[index1] = _hasBeenWritten[index2] = true;
        _writeAccess += 2;
        _hasBeenReaden[index1] = _hasBeenReaden[index2] = true;
        _readAccess += 2;
        operation_sleep(2);
    }

    ElementType operator[](int index) const
    {
        return get(index);
    }

    ElementType& operator[](int index)
    {
        return get(index);
    }

    void operator=(const TemplateArray<ElementType>& other)
    {
        _data.assign(other._data.begin(), other._data.end());
        _hasBeenReaden.assign(other._hasBeenReaden.begin(),other._hasBeenReaden.end());
        _hasBeenWritten.assign(other._hasBeenWritten.begin(),other._hasBeenWritten.end());
        _readAccess = other._readAccess;
        _writeAccess = other._writeAccess;
    }

    void insert(const int index, const ElementType& value)
    {
        int i=index;
        while(i < _data.size()-1 && _data[i] >= 0)
            i++;
        while (i > index)
        {
            _data[i]=_data[i-1];
            ++_writeAccess;
            _hasBeenWritten[i]=true;
            --i;
        }
        _data[index]=value;
        ++_writeAccess;
        _hasBeenWritten[index]=true;
    }

    const std::vector<ElementType>& data() const
    {
        return _data;
    }

protected:
    std::vector<ElementType> _data;
};

class Array : public TemplateArray<int>
{
public:
    Array(size_t size=100);
    Array(const TemplateArray<int>& other);
    Array(const std::vector<int>& other);

    virtual ~Array() {}

    void clear(int start, int end, int value=-1)
    {
        for (int i=start; i<end; ++i)
            _data[i] = -1;
    }

    size_t effectiveSize() const;


    void fill(const int value);
    void fillRandom(const int min, const int max);
    void fillSortedRandom(const int min, const int max);

    std::vector<int>::iterator begin();
    std::vector<int>::iterator end();

    QString toString() const;
};

#endif // ARRAY_H
