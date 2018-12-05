#include "Types.h"
#include <memory>
#include <iostream>

template <typename T>
class ImageBase : public std::enable_shared_from_this<ImageBase<T>>
{
private:
    std::vector<T> _data;
    Size    _size;

public:
    ImageBase(Size size, T defaultVal);
    typedef std::shared_ptr<ImageBase<T>> Ptr;
    Ptr     getPtr()                           {return std::enable_shared_from_this<ImageBase<T>>::shared_from_this();}
    Size    size  ()                           {return _size;}
    T       read  (COORD x, COORD y);
    void    write (COORD x, COORD y, T value)  {_data.at(y*_size.x + x) = value;}
};

template <typename T>
inline ImageBase<T>::ImageBase(Size size, T defaultVal)
{
    _size = size;
    _data.resize(_size.area(),defaultVal);
}

template <typename T>
T ImageBase<T>::read (COORD x, COORD y)
{
    if(x<size().x && y<size().y)
        return _data.at(y*_size.x + x);
    else
        std::cerr << "out_of_range error"<<std::endl;
    return T(0);
}