#include "Types.h"
#include <vector>

template <typename T>
class ImageBase
{
private:
    std::vector<T> _data;
    Size    _size;
public:
    ImageBase(Size size, T defaultVal);
    Size size  ()                          {return _size;}
    T    read  (COORD x, COORD y)          {return _data.at(y*_size.x + x);}
    void write (COORD x, COORD y, T value) {_data.at(y*_size.x + x) = value;}
};

template <typename T>
ImageBase<T>::ImageBase(Size size, T defaultVal)
{
    _size = size;
    for(ELEMENT i=0; i<_size.area(); i++)
    {
        _data.push_back(T(defaultVal));
    }
}