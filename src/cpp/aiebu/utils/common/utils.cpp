// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <filesystem>
#include "utils.h"

#include <boost/geometry/geometry.hpp>
#include <boost/format.hpp>

// Temporary color objects until the supporting color library becomes available
namespace ec {
  class fgcolor
    {
    public:
      fgcolor(uint8_t _color) : m_color(_color) {};
      std::string string() const { return "\033[38;5;" + std::to_string(m_color) + "m"; }
      static const std::string reset() { return "\033[39m"; };
      friend std::ostream& operator <<(std::ostream& os, const fgcolor & _obj) { return os << _obj.string(); }

   private:
     uint8_t m_color;
  };

  class bgcolor
    {
    public:
      bgcolor(uint8_t _color) : m_color(_color) {};
      std::string string() const { return "\033[48;5;" + std::to_string(m_color) + "m"; }
      static const std::string reset() { return "\033[49m"; };
      friend std::ostream& operator <<(std::ostream& os, const bgcolor & _obj) { return  os << _obj.string(); }

   private:
     uint8_t m_color;
  };
}
// ------ C O L O R S ---------------------------------------------------------
static const uint8_t FGC_HEADER           = 3;   // 3
static const uint8_t FGC_HEADER_BODY      = 111; // 111

static const uint8_t FGC_USAGE_BODY       = 252; // 252

static const uint8_t FGC_OPTION           = 65;  // 65
static const uint8_t FGC_OPTION_BODY      = 111; // 111

static const uint8_t FGC_SUBCMD           = 140; // 140
static const uint8_t FGC_SUBCMD_BODY      = 111; // 111

static const uint8_t FGC_POSITIONAL       = 140; // 140
static const uint8_t FGC_POSITIONAL_BODY  = 111; // 111

static const uint8_t FGC_OOPTION          = 65;  // 65
static const uint8_t FGC_OOPTION_BODY     = 70;  // 70

static const uint8_t FGC_EXTENDED_BODY    = 70;  // 70

bool
aiebu::utilities::is_escape_codes_disabled() {
  return false;
}

class FormatHelper {
private:
  static FormatHelper* m_instance;
  FormatHelper() {
    this->fgc_header       = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_HEADER).string();
    this->fgc_headerBody   = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_HEADER_BODY).string();
    this->fgc_commandBody  = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_SUBCMD).string();
    this->fgc_usageBody    = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_USAGE_BODY).string();
    this->fgc_ooption      = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_OOPTION).string();
    this->fgc_ooptionBody  = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_OOPTION_BODY).string();
    this->fgc_poption      = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_POSITIONAL).string();
    this->fgc_poptionBody  = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_POSITIONAL_BODY).string();
    this->fgc_extendedBody = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_EXTENDED_BODY).string();
    this->fgc_reset        = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor::reset();
    this->fgc_optionName   = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_OPTION).string();
    this->fgc_optionBody   = aiebu::utilities::is_escape_codes_disabled() ? "" : ec::fgcolor(FGC_OPTION_BODY).string();
  }

public:
  std::string fgc_optionName;
  std::string fgc_optionBody;
  std::string fgc_header;
  std::string fgc_headerBody;
  std::string fgc_commandBody;
  std::string fgc_usageBody;
  std::string fgc_ooption;
  std::string fgc_ooptionBody;
  std::string fgc_poption;
  std::string fgc_poptionBody;
  std::string fgc_extendedBody;
  std::string fgc_reset;

  static FormatHelper& instance();
};

FormatHelper* FormatHelper::m_instance = nullptr;

FormatHelper&
FormatHelper::instance() {
  static FormatHelper formatHelper;
  return formatHelper;
}


// ------ S T A T I C   V A R I A B L E S -------------------------------------
static unsigned int m_maxColumnWidth = 100;

// ------ F U N C T I O N S ---------------------------------------------------
static bool
isPositional(const std::string &_name,
             const po::positional_options_description & _pod)
{
  // Look through the list of positional arguments
  for (unsigned int index = 0; index < _pod.max_total_count(); ++index) {
    if ( _name.compare(_pod.name_for_position(index)) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * An enumeration to describe the type of argument a given
 * program option represents.
 * This determines the output order of options in the usage string
 */
enum OptionDescriptionFlagType {
  short_required = 0,
  long_required,
  short_required_arg,
  long_required_arg,
  short_simple,
  long_simple,
  short_arg,
  long_arg,
  positional,
  flag_type_size
};

static enum OptionDescriptionFlagType
get_option_type(const boost::shared_ptr<po::option_description>& option,
                const po::positional_options_description& _pod)
{
  const static int SHORT_OPTION_STRING_SIZE = 2;
  std::string optionDisplayName = option->canonical_display_name(po::command_line_style::allow_dash_for_short);

  if (isPositional(optionDisplayName, _pod))
    return positional;

  if (option->semantic()->is_required()) {
    if (option->semantic()->max_tokens() == 0) {
      if (optionDisplayName.size() == SHORT_OPTION_STRING_SIZE)
        return short_required;

      return long_required;
    } else {
      if (optionDisplayName.size() == SHORT_OPTION_STRING_SIZE)
        return short_required_arg;

      return long_required_arg;
    }
  }

  if (option->semantic()->max_tokens() == 0) {  // Parse for simple flags
    if (optionDisplayName.size() == SHORT_OPTION_STRING_SIZE)
      return short_simple;

    return long_simple;
  }
  else { // Parse for flags with arguments
    if (optionDisplayName.size() == SHORT_OPTION_STRING_SIZE)
      return short_arg;

    return long_arg;
  }
}

static std::string
create_option_string(enum OptionDescriptionFlagType optionType,
                     const boost::shared_ptr<po::option_description>& option,
                     bool remove_long_opt_dashes)
{
  const std::string& shortName = option->canonical_display_name(po::command_line_style::allow_dash_for_short);
  const std::string& longName =
      remove_long_opt_dashes ? option->long_name() : option->canonical_display_name(po::command_line_style::allow_long);
  switch (optionType) {
    case short_simple:
      return boost::str(boost::format("%s") % shortName[1]);
      break;
    case long_simple:
      return boost::str(boost::format("[%s]") % longName);
      break;
    case short_arg:
      return boost::str(boost::format("[%s arg]") % shortName);
      break;
    case long_arg:
      return boost::str(boost::format("[%s arg]") % longName);
      break;
    case short_required:
      return boost::str(boost::format("%s") % shortName);
      break;
    case long_required:
      return boost::str(boost::format("%s") % longName);
      break;
    case short_required_arg:
      return boost::str(boost::format("%s arg") % shortName);
      break;
    case long_required_arg:
      return boost::str(boost::format("%s arg") % longName);
      break;
    case positional:
      return boost::str(boost::format("%s") % shortName);
      break;
    case flag_type_size:
      throw std::runtime_error("Invalid argument setup detected");
      break;
  }
  throw std::runtime_error("Invalid argument setup detected");
}


std::string
aiebu::utilities::create_usage_string(const po::options_description &od,
                                      const po::positional_options_description &pod,
                                      bool remove_long_opt_dashes)
{
  // Create list of buffers to store each argument type
  std::vector<std::stringstream> buffers(flag_type_size);

  const auto& options = od.options();

  for (const auto& option : options) {
    const auto optionType = get_option_type(option, pod);
    const auto optionString = create_option_string(optionType, option, remove_long_opt_dashes);
    const auto is_buffer_empty = buffers[optionType].str().empty();
    // The short options have a bracket surrounding all options
    if ((optionType == short_simple) && is_buffer_empty)
      buffers[optionType] << "[-";
    // Add spaces only after the first character to simplify upper level formatting
    else if ((optionType != short_simple) && !is_buffer_empty)
      buffers[optionType] << " ";

    buffers[optionType] << boost::format("%s") % optionString;
  }

  // The short simple options have a bracket surrounding all options
  if (!buffers[short_simple].str().empty())
    buffers[short_simple] << "]";

  std::stringstream outputBuffer;
  for (const auto& buffer : buffers) {
    if (!buffer.str().empty()) {
      // Add spaces only after the first buffer to simplify upper level formatting
      if (!outputBuffer.str().empty())
        outputBuffer << " ";
      outputBuffer << buffer.str();
    }
  }

  return outputBuffer.str();
}

std::string
aiebu::utilities::wrap_paragraphs(const std::string & unformattedString,
                                  unsigned int indentWidth,
                                  unsigned int columnWidth,
                                  bool indentFirstLine) {
  std::vector<std::string> lines;

  // Process the string
  std::string workingString;

  for (const auto &entry : unformattedString) {
    // Do we have a new line added by the user
    if (entry == '\n') {
      lines.push_back(workingString);
      workingString.clear();
      continue;
    }

    workingString += entry;

    // Check to see if this string is too long
    if (workingString.size() >= columnWidth) {
      // Find the beginning of the previous 'word'
      auto index = workingString.find_last_of(" ");

      // None found, keep on adding characters till we find a space
      if (index == std::string::npos)
        continue;

      // Add the line and populate the next line
      lines.push_back(workingString.substr(0, index));
      workingString = workingString.substr(index + 1);
    }
  }

  if (!workingString.empty())
    lines.push_back(workingString);

  // Early exit, nothing here
  if (lines.size() == 0)
    return std::string();

  // -- Build the formatted string
  std::string formattedString;

  // Iterate over the lines building the formatted string
  const std::string indention(indentWidth, ' ');
  auto iter = lines.begin();
  while (iter != lines.end()) {
    // Add an indention
    if (iter != lines.begin() || indentFirstLine)
      formattedString += indention;

    // Add formatted line
    formattedString += *iter;

    // Don't add a '\n' on the last line
    if (++iter != lines.end())
      formattedString += "\n";
  }

  return formattedString;
}

void
aiebu::utilities::report_commands_help(const std::string &executable,
                                       const std::string &description,
                                       const po::options_description& option_description,
                                       const target_collection& /*sub_targets*/)
{
  const auto& fh = FormatHelper::instance();

  // -- Command description
  {
    static const std::string key = "DESCRIPTION: ";
    auto formattedString = aiebu::utilities::wrap_paragraphs(description, static_cast<unsigned int>(key.size()),
                             m_maxColumnWidth - static_cast<unsigned int>(key.size()), false);
    boost::format fmtHeader(fh.fgc_header + "\n" + key + fh.fgc_headerBody + "%s\n" + fh.fgc_reset);
    if ( !formattedString.empty() )
      std::cout << fmtHeader % formattedString;
  }

  // -- Command usage
  po::positional_options_description emptyPOD;
  std::string usage = aiebu::utilities::create_usage_string(option_description, emptyPOD);
  boost::format fmtUsage(fh.fgc_header + "\nUSAGE: " + fh.fgc_usageBody + "%s %s\n" + fh.fgc_reset);
  std::cout << fmtUsage % executable % usage;


  report_option_help("OPTIONS", option_description, false);
}

static std::string
create_option_format_name(const po::option_description * option,
                          bool report_parameter = true,
                          bool remove_long_opt_dashes = false)
{
  if (option == nullptr)
    return "";

  std::string option_display_name = option->canonical_display_name(po::command_line_style::allow_dash_for_short);

  // Determine if we really got the "short" name (might not exist and a long was returned instead)
  if (!option_display_name.empty() && option_display_name[0] != '-')
    option_display_name.clear();

  // Get the long name (if it exists)
  std::string longName = option->canonical_display_name(po::command_line_style::allow_long);
  if ((longName.size() > 2) && (longName[0] == '-') && (longName[1] == '-')) {
    if (!option_display_name.empty())
      option_display_name += ", ";

    option_display_name += remove_long_opt_dashes ? option->long_name() : longName;
  }

  if (report_parameter && !option->format_parameter().empty())
    option_display_name += " " + option->format_parameter();

  return option_display_name;
}

static void
print_options(std::stringstream& stream,
              const po::options_description& options,
              const bool report_param,
              const bool remove_long_dashes)
{
  const auto& fh = FormatHelper::instance();
  boost::format fmtOption(fh.fgc_optionName + "  %-18s " + fh.fgc_optionBody + "- %s\n" + fh.fgc_reset);
  for (auto & option : options.options()) {
    std::string optionDisplayFormat = create_option_format_name(option.get(), report_param, remove_long_dashes);
    const unsigned int optionDescTab = 23;
    auto formattedString = aiebu::utilities::wrap_paragraphs(option->description(), optionDescTab, m_maxColumnWidth - optionDescTab, false);
    stream << fmtOption % optionDisplayFormat % formattedString;
  }
}

void
aiebu::utilities::report_option_help(const std::string & group_name,
                                     const po::options_description& option_description,
                                     const bool report_parameter,
                                     const bool remove_long_opt_dashes)
{
  const auto& fh = FormatHelper::instance();

  // Determine if there is anything to report
  if (option_description.options().empty())
    return;

  // Report option group name (if defined)
  boost::format fmtHeader(fh.fgc_header + "\n%s:\n" + fh.fgc_reset);
  if ( !group_name.empty() )
    std::cout << fmtHeader % group_name;

  po::options_description common_options(option_description);

  // Generate the common options
  std::stringstream commonOutput;
  print_options(commonOutput, common_options, report_parameter, remove_long_opt_dashes);

  std::cout << commonOutput.str();

}

void
aiebu::utilities::report_target_help(const std::string& executable_name,
                                     const std::string& starget,
                                     const std::string& description,
                                     const po::options_description& option_description,
                                     bool /*remove_long_opt_dashes*/)
{
  const auto& fh = FormatHelper::instance();

  // -- Command
  boost::format fmtCommand(fh.fgc_header + "\nTARGET: " + fh.fgc_commandBody + "%s\n" + fh.fgc_reset);
  if (!starget.empty())
    std::cout << fmtCommand % starget;

  // -- Command description
  {
    auto formattedString = aiebu::utilities::wrap_paragraphs(description, 15, m_maxColumnWidth, false);
    boost::format fmtHeader(fh.fgc_header + "\nDESCRIPTION: " + fh.fgc_headerBody + "%s\n" + fh.fgc_reset);
    if (!formattedString.empty())
      std::cout << fmtHeader % formattedString;
  }

  // -- Command usage
  po::positional_options_description emptyPOD;
  std::string usage = aiebu::utilities::create_usage_string(option_description, emptyPOD);
  boost::format fmtUsage(fh.fgc_header + "\nUSAGE: " + fh.fgc_usageBody + "%s %s\n" + fh.fgc_reset);
  std::string command = executable_name + " -t " + starget;
  std::cout << fmtUsage % command % usage;

  // -- Options
  report_option_help("OPTIONS", option_description, false);
}

std::vector<std::string>
aiebu::utilities::process_arguments(po::variables_map& vm,
                                    po::command_line_parser& parser,
                                    const po::options_description& options,
                                    bool validate_arguments)
{
  // Add unregistered "option"" that will catch all extra positional arguments
  po::options_description all_options(options);
  all_options.add_options()("__unreg", po::value<std::vector<std::string> >(), "Holds all unregistered options");

  // Parse the given options and hold onto the results
  auto parsed_options = parser.options(all_options).allow_unregistered().run();

  if (validate_arguments) {
    // This variables holds options denoted with a '-' or '--' that were not registered
    const auto unrecognized_options = po::collect_unrecognized(parsed_options.options, po::exclude_positional);
    // Parse out all extra positional arguments from the boost results
    // This variable holds arguments that do not have a '-' or '--' that did not have a registered positional space
    std::vector<std::string> extra_positionals;
    for (const auto& option : parsed_options.options) {
      if (boost::equals(option.string_key, "__unreg"))
        // Each option is a vector even though most of the time they contain only one element
        for (const auto& bad_option : option.value)
          extra_positionals.push_back(bad_option);
    }

    // Throw an exception if we have any unknown options or extra positionals
    if (!unrecognized_options.empty() || !extra_positionals.empty()) {
      std::string error_str;
      error_str.append("Unrecognized arguments:\n");
      for (const auto& option : unrecognized_options)
        error_str.append(boost::str(boost::format("  %s\n") % option));
      for (const auto& option : extra_positionals)
        error_str.append(boost::str(boost::format("  %s\n") % option));
      throw po::error(error_str);
    }
  }

  // Parse the options into the variable map
  // If an exception occurs let it bubble up and be handled elsewhere
  po::store(parsed_options, vm);
  po::notify(vm);

  // Return all the unrecognized arguments for use in a lower level command if needed
  return po::collect_unrecognized(parsed_options.options, po::include_positional);
}

