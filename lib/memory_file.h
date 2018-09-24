#ifndef _QMJ_MEMORY_FILE_
#define _QMJ_MEMORY_FILE_

#include <vector>
#include <string>
#include <memory>

namespace qmj
{

class memory_file
{
  public:
    typedef memory_file self;

    // defaule constructor
    memory_file();

    // construct by @v_file
    memory_file(const std::vector<std::string> &v_file);

    // construct by left-value @v_file
    memory_file(std::vector<std::string> &&v_file);

    // construct by self @other, @start_pos,@end_pos
    memory_file(const self &other,
                const size_t start_pos,
                const size_t end_pos);

    // copy constructor
    memory_file(const self &) = delete;

    // assignment constructor
    self &operator=(const self &) = delete;

    // get current index of file
    size_t get_index() const;

    // get abs index of origin file
    size_t get_absindex() const;

    // @return @index line of file
    std::string getline(const size_t index) const;

    // @return @abs_index line of origin file
    std::string get_absline(const size_t abs_index) const;

    // @return line num of file
    size_t size() const;

    // @return line num of origin file
    size_t origin_size() const;

    // @return true if read end of file
    bool is_eof() const;

    // return true if read next line successfully
    bool getline();

    // return true if read next line successfully,and wirte it to @buf
    bool getline(std::string &buf);

    void reopend();

  private:
    // @return start pos of file
    size_t __get_start_pos() const;

    // @return current index of file
    size_t __get_cur_file_index() const;

    // @return abs index of file
    size_t __get_abs_file_index() const;

    // @return new index of file
    size_t __set_file_index(const size_t index);

    // @return total line num of file
    size_t __get_file_size() const;

    // @return total line num of origin file
    size_t __get_origin_file_size() const;

    // @return ture if move index of file to next line successfully
    bool __set_next_index();

    // set start and end pos of file by @start,@end
    void __set_file_pos(size_t start, size_t end);

    // @return line of current index in file
    std::string __get_cur_line() const;

    // @return @index line of file
    std::string __get_index_line_imple(const size_t index) const;

    // @return abs @abs_index line of file
    std::string __get_absindex_line_imple(const size_t abs_index) const;

  private:
    // container of file
    std::shared_ptr<std::vector<std::string>> ptr_v_file;
    // start position of file
    size_t start_pos;
    // end postion of file
    size_t end_pos;
    // current line index of file
    size_t file_index;
};

} // namespace qmj

#endif