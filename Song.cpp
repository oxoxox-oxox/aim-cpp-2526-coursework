
#include <string>
#include <iostream>
#include "Song.h"
#include <cctype>

namespace
{

    // 去空白部分
    std::string trim_copy(const std::string &s)
    {
        size_t start = 0;
        size_t end = s.size();
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        {
            start++;
        }
        while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        {
            end--;
        }
        return s.substr(start, end - start);
    }

    // 全小写
    std::string to_lower_copy(const std::string &s)
    {
        std::string s_new = s;
        for (size_t i = 0; i < s.size(); i++)
        {
            int asc = static_cast<int>(s[i]);
            s_new[i] = std::tolower(static_cast<unsigned char>(s[i]));
        }
        return s_new;
    }

    // 数组转字符串
    std::string join_tags(const std::vector<std::string> &tags)
    {
        std::string result;

        for (size_t i = 0; i < tags.size(); ++i)
        {
            result += tags[i];
            if (i < tags.size() - 1)
            {
                result += ", "; // 在每个元素后加上逗号和空格，最后一个不加
            }
        }
        return result;
    }
}

int Song::next_id_ = 1;

// 构造函数

Song::Song(const std::string &title,
           const std::string &artist,
           int duration_sec,
           int rating)
{
    std::string t = trim_copy(title);
    std::string a = trim_copy(artist);
    if (t.empty())
    {
        std::cout << "[错误] 标题不能为空\n";
        valid_ = false;
        return;
    }

    if (a.empty())
    {
        std::cout << "[错误] 艺人不能为空\n";
        valid_ = false;
        return;
    }

    if (duration_sec <= 0)
    {
        std::cout << "[错误] 时长必须为正整数（秒）\n";
        valid_ = false;
        return;
    }

    if (rating < 1 || rating > 5)
    {
        std::cout << "[错误] 评分必须在 1...5 之间\n";
        valid_ = false;
        return;
    }

    id_ = next_id_++;
    title_ = t;
    artist_ = a;
    duration_sec_ = duration_sec;
    rating_ = rating;
    valid_ = true;
}

// 其他函数

bool Song::set_title(const std::string &t)
{
    std::string t_new = trim_copy(t);
    if (t_new.empty())
    {
        std::cout << "[提示] 标题不能为空，已忽略本次修改\n";
        return false;
    }
    else
    {
        title_ = t_new;
        return true;
    }
}

bool Song::set_artist(const std::string &a)
{
    std::string a_new = trim_copy(a);
    if (a_new.empty())
    {
        std::cout << "[提示] 艺人不能为空，已忽略本次修改\n";
        return false;
    }
    else
    {
        artist_ = a_new;
        return true;
    }
}

bool Song::set_duration(int sec)
{
    if (sec <= 0)
    {
        std::cout << "[提示] 时长需为正整数，已忽略本次修改\n";
        return false;
    }
    else
    {
        duration_sec_ = sec;
        return true;
    }
}

bool Song::set_rating(int r)
{
    if (r < 1 || r > 5)
    {
        std::cout << "[提示] 评分需在 1..5，已忽略本次修改\n";
        return false;
    }
    else
    {
        rating_ = r;
        return true;
    }
}

bool Song::add_tag(const std::string &tag)
{
    std::string tag_add = trim_copy(tag);
    if (tag_add.empty())
    {
        std::cout << "[提示] 空标签已忽略。\n";
        return false;
    }
    for (size_t i = 0; i < tags_.size(); i++)
    {
        if (to_lower_copy(tag_add) == to_lower_copy(tags_[i]))
        {
            std::cout << "[提示] 标签已存在（忽略大小写）\n";
            return false;
        }
    }
    tags_.push_back(tag_add);
    return true;
}

bool Song::remove_tag(const std::string &tag)
{
    std::string tag_remove = to_lower_copy(trim_copy(tag));
    for (size_t i = 0; i < tags_.size(); i++)
    {
        if (tag_remove == to_lower_copy(tags_[i]))
        {
            tags_.erase(tags_.begin() + i);
            return true;
        }
    }
    std::cout << "[提示] 未找到该标签\n";
    return false;
}

bool Song::matches_keyword(const std::string &kw) const
{
    std::string kw_new = to_lower_copy(trim_copy(kw));
    if (kw_new.empty())
    {
        return false;
    }
    std::string title_new = to_lower_copy(title_);
    std::string artist_new = to_lower_copy(artist_);
    if (title_new.find(kw_new) != std::string::npos)
    {
        return true;
    }
    else if (artist_new.find(kw_new) != std::string::npos)
    {
        return true;
    }
    for (size_t i = 0; i < tags_.size(); i++)
    {
        if (to_lower_copy(tags_[i]).find(kw_new) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const Song &s)
{
    os << "[#" << s.id_ << "] "
       << s.artist_ << " - " << s.title_
       << " (" << s.duration_sec_ << "s) "
       << std::string(s.rating_, '*');

    if (!s.tags_.empty())
    {
        os << "  [tags: " << join_tags(s.tags_) << "]";
    }

    return os;
}

bool operator<(const Song &a, const Song &b)
{
    if (a.rating_ > b.rating_)
    {
        return true;
    }
    else if (a.rating_ < b.rating_)
    {
        return false;
    }
    else if (a.title_ < b.title_)
    {
        return true;
    }
    else if (a.title_ > b.title_)
    {
        return false;
    }
    else
    {
        return a.id_ < b.id_;
    }
}