#include "memory_file.h"

namespace qmj
{

memory_file::memory_file() : start_pos(0), end_pos(0), file_index(0) {}

memory_file::memory_file(const std::vector<std::string> &v_file)
    : ptr_v_file(new std::vector<std::string>(v_file)),
      start_pos(0),
      end_pos(ptr_v_file->size()),
      file_index(0) {}

memory_file::memory_file(std::vector<std::string> &&v_file)
    : ptr_v_file(std::make_shared<std::vector<std::string>>(std::move(v_file))),
      start_pos(0),
      end_pos(ptr_v_file->size()),
      file_index(0) {}

memory_file::memory_file(const self &other, const size_t start,
                         const size_t end)
    : ptr_v_file(other.ptr_v_file),
      file_index(0)
{
    __set_file_pos(start, end);
}

size_t memory_file::get_index() const
{
    return __get_cur_file_index();
}

size_t memory_file::get_absindex() const
{
    return __get_abs_file_index();
}

std::string memory_file::getline(const size_t index) const
{
    return __get_index_line_imple(index);
}

std::string memory_file::get_absline(const size_t abs_index) const
{
    return __get_absindex_line_imple(abs_index);
}

size_t memory_file::size() const
{
    return __get_file_size();
}

bool memory_file::is_eof() const
{
    return __get_cur_file_index() == __get_file_size();
}

bool memory_file::getline()
{
    return __set_next_index();
}

bool memory_file::getline(std::string &buf)
{
    if (is_eof() == true)
        return false;
    buf = __get_cur_line();
    __set_next_index();
    return true;
}

void memory_file::reopend()
{
    __set_file_index(0);
}

size_t memory_file::__get_cur_file_index() const
{
    return this->file_index;
}

size_t memory_file::__get_abs_file_index() const
{
    return this->file_index + this->start_pos;
}

size_t memory_file::__set_file_index(const size_t index)
{
    return (this->file_index =
                index <= this->__get_file_size()
                    ? index
                    : this->__get_file_size());
}

size_t memory_file::__get_file_size() const
{
    return this->end_pos - this->start_pos;
}

size_t memory_file::origin_size() const
{
    return __get_origin_file_size();
}

size_t memory_file::__get_start_pos() const
{
    return this->start_pos;
}

size_t memory_file::__get_origin_file_size() const
{
    return this->ptr_v_file->size();
}

bool memory_file::__set_next_index()
{
    size_t index = this->__get_cur_file_index();
    return index != this->__set_file_index(index + 1);
}

void memory_file::__set_file_pos(size_t start, size_t end)
{
    if (start > this->ptr_v_file->size())
        start = this->ptr_v_file->size();
    if (end > this->ptr_v_file->size())
        end = this->ptr_v_file->size();
    if (start > end)
        start_pos = end_pos = ptr_v_file->size();
    this->start_pos = start;
    this->end_pos = end;
}

std::string memory_file::__get_cur_line() const
{
    return __get_index_line_imple(__get_cur_file_index());
}

std::string memory_file::__get_index_line_imple(const size_t index) const
{
    return __get_absindex_line_imple(this->start_pos + index);
}

std::string memory_file::__get_absindex_line_imple(const size_t abs_index) const
{
    return this->ptr_v_file->operator[](abs_index);
}

} // namespace qmj