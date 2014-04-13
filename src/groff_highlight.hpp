/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing in LICENSE file, or at:
    http://www.opensource.org/licenses/MIT

  File: groff_highlight.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

namespace groff_detail
{
  enum class fmt { bold, italic };
  void highlight_impl(std::string &l, std::string const &pattern, fmt const &format)
  {
    auto const found(l.find(pattern));
    if(found != std::string::npos)
    {
      switch(format)
      {
        case fmt::bold:
          l.insert(found, "\\fB");
          l.insert(found + pattern.size() + 3, "\\fP");
          break;
        case fmt::italic:
          l.insert(found, "\\fI");
          l.insert(found + pattern.size() + 3, "\\fP");
          break;
        default:
          throw std::invalid_argument("unknown highlight format");
      }
    }
  }

  void highlight(std::vector<std::string> &lines)
  {
    std::string section;
    for(auto &l : lines)
    {
      /* Determine what section we're in. */
      if(l.size() > 3 && l.substr(0, 3) == ".SH")
      { section = l.substr(4); }

      /* Color standard release annotations. */
      highlight_impl(l, "(C++11)", fmt::italic);
      highlight_impl(l, "(C++14)", fmt::italic);
      highlight_impl(l, "(until C++11)", fmt::italic);
      highlight_impl(l, "(since C++11)", fmt::italic);
      highlight_impl(l, "(until C++14)", fmt::italic);
      highlight_impl(l, "(since C++14)", fmt::italic);

      /* Description of members. */
      highlight_impl(l, "[static]", fmt::bold);
      highlight_impl(l, "(public member function)", fmt::italic);
      highlight_impl(l, "(public static member function)", fmt::italic);
      highlight_impl(l, "(public member)", fmt::italic);
      highlight_impl(l, "(public member constant)", fmt::italic);
      highlight_impl(l, "(public static member)", fmt::italic);
      highlight_impl(l, "(public static member constant)", fmt::italic);
      highlight_impl(l, "(public member class)", fmt::italic);
      highlight_impl(l, "(class template specialization)", fmt::italic);
      highlight_impl(l, "(class)", fmt::italic);
      highlight_impl(l, "(class template)", fmt::italic);
      highlight_impl(l, "(function)", fmt::italic);
      highlight_impl(l, "(function template)", fmt::italic);
      highlight_impl(l, "(strong exception guarantee)", fmt::italic);

      /* Overload numbers. */
      static size_t constexpr const max_overloads{ 43 };
      for(size_t i{}; i < max_overloads; ++i)
      {
        /* O(1) shows up in algorithmic complexities. */
        if(i == 1 && l.find("O(1)") != std::string::npos)
        { continue; }
        /* Examples may initialize variables with foo(n). */
        else if(section.find("Example") != std::string::npos)
        { continue; }

        highlight_impl(l, "(" + std::to_string(i) + ")", fmt::bold);
      }
    }
  }
}