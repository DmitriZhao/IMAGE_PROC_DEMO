#include "Types.h"
#include <memory>
#include <iostream>

template <typename T>
class ImageBase
{
private:
    std::vector<T> _data;

protected:
    Size    _size;

public:
    ImageBase(const Size& size, const T& defaultVal);
    ImageBase(const Size& size, std::initializer_list<T> list);
    Size         size  () const                     {return _size;}
    T            read  (COORD x, COORD y) const     {return _data.at(y*_size.x + x);}
    void         write (COORD x, COORD y, T value)  {_data.at(y*_size.x + x) = value;}
};

template <typename T>
inline ImageBase<T>::ImageBase(const Size& size, const T& defaultVal)
{
    _size = size;
    _data.resize(_size.area(),defaultVal);
}

template <typename T>
inline ImageBase<T>::ImageBase(const Size& size, std::initializer_list<T> list)
{
    _size = size;
    _data.reserve(_size.area());
    for(auto i : list)
        _data.push_back(i);
}