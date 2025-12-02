#ifndef AIEBU_COMMON_REGEX_WRAPPER_H_
#define AIEBU_COMMON_REGEX_WRAPPER_H_

/**
 * @file regex_wrapper.h
 * @brief Unified regex API wrapper supporting both std::regex and boost::regex
 * 
 * This wrapper allows transparent switching between std::regex and boost::regex
 * based on platform requirements:
 * - Ubuntu 22.04 and earlier: uses std::regex (better integration with system)
 * - Ubuntu 24.04+: uses boost::regex (header-only, no packaging issues)
 * - Windows: uses boost::regex (consistent with Boost ecosystem)
 * 
 * The wrapper is controlled by the USE_BOOST_REGEX CMake option.
 */

#ifdef USE_BOOST_REGEX
  #include <boost/regex.hpp>
#else
  #include <regex>
#endif

namespace aiebu {

#ifdef USE_BOOST_REGEX
  // Use Boost.Regex
  using regex = boost::regex;
  using smatch = boost::smatch;
  using cmatch = boost::cmatch;
  using sregex_iterator = boost::sregex_iterator;
  using regex_error = boost::regex_error;
  
  // Import functions into aiebu namespace
  using boost::regex_match;
  using boost::regex_search;
  using boost::regex_replace;
  
  // Regex constants
  namespace regex_constants {
    using namespace boost::regex_constants;
  }
  
#else
  // Use std::regex
  using regex = std::regex;
  using smatch = std::smatch;
  using cmatch = std::cmatch;
  using sregex_iterator = std::sregex_iterator;
  using regex_error = std::regex_error;
  
  // Import functions into aiebu namespace
  using std::regex_match;
  using std::regex_search;
  using std::regex_replace;
  
  // Regex constants
  namespace regex_constants {
    using namespace std::regex_constants;
  }
  
#endif

} // namespace aiebu

#endif // AIEBU_COMMON_REGEX_WRAPPER_H_

