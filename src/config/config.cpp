#include <fstream>
#include <streambuf>
#include <queue>
#include <cassert>


#include <YACL/settings.h>
#include <YACL/settings_ptr.h>
#include <YACL/config.h>

#include <helper/utils.h>
#include <grammar/grammar.h>
#include <type_conversation/converter.h>


yacl::Config::DoubleCharSize_tPair yacl::Config::fillNameAndMarkFields(
  const std::string &raw_config, std::string *name) noexcept {
  DoubleCharSize_tPair pair;

  for (size_t i = 0; i < raw_config.size(); ++i) {
    // assign first {, its top
    if (pair.first_open_bracket != '{' && raw_config[i] == '{') {
      pair.first_open_bracket = raw_config[i];
      pair.ind_first = i;
    }

    // getting all chars till meet first {
    // all chars before it, be our setting_name
    if (pair.first_open_bracket != '{' && raw_config[i] != '=') {
      name->push_back(raw_config[i]);
    }

    // last }, be closed to top 
    if (raw_config[i] == '}') {
      pair.last_closed_bracket = raw_config[i];

      pair.ind_second = i;
    }
  }
  trim(*name);
  return pair;
}

yacl::SettingsUniquePtr yacl::Config::parseConfigFromFile(
  const std::string &path) noexcept {
  std::ifstream t(path);
  std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
  return parseConfig(std::move(str));
}

yacl::SettingsUniquePtr yacl::Config::parseConfig(std::string conf) noexcept {

  std::queue<PairPrior> brackets_to_be_parsed;
  conf = "root = {" + conf + '}';
  const auto super_parent = new Setting(nullptr, 0);

  std::map<uint16_t, std::vector<Setting*>> levels;

  brackets_to_be_parsed.push({0, conf});
  levels[0].push_back(super_parent);

  while (!brackets_to_be_parsed.empty()) {
    std::string raw_config = brackets_to_be_parsed.front().content;
    uint16_t cur_dep = brackets_to_be_parsed.front().prior;

    brackets_to_be_parsed.pop();

    std::string name;
    const DoubleCharSize_tPair pair = fillNameAndMarkFields(raw_config, &name);

    std::vector<std::string> inner_fields = 
      smart_bracket_split(
        raw_config.substr(pair.ind_first + 1,
                          pair.ind_second - pair.ind_first - 1
        ), 
      ',', 
      ';'
    );

    std::vector<size_t> idx_for_pop;

    // parse and delete uneed
    Setting *parent = nullptr;
    for (Setting *sett : levels[cur_dep]) {
      if (sett && !sett->filled) {
        parent = sett;
        break;
      }
    }
    assert(parent != nullptr &&
      ("FAILED TO PARSE CONFIG, PARENT == NULLPTR FOR RAW '" + raw_config + "'")
      .c_str());

    parent->setting_name = name;
    parent->filled = true;
    parent->anon = name.empty();

    for (size_t i = 0; i < inner_fields.size(); ++i) {
      if (inner_fields[i].find('{') == std::string::npos) {
        auto parsed_field =
          vectorFieldsToMapFields(std::
                                  move(parseBracketIntoFields(inner_fields[i
                                                              ])));
        parent->fields.insert(
                              std::make_move_iterator(parsed_field.begin()),
                              std::make_move_iterator(parsed_field.end()));
        idx_for_pop.push_back(i);
      }
    }

    vector_multi_erase(inner_fields, idx_for_pop);

    for (const auto &s : inner_fields) {
      uint16_t new_dep = parent->depth + 1;
      brackets_to_be_parsed.push({new_dep, s});
      levels[new_dep].push_back(new Setting(parent, new_dep));
    }

    for (Setting *sett : levels[cur_dep + 1]) {
      if (!sett->father)
        break;

      if (sett->father == parent)
        parent->inner_graph_vec.push_back(sett);
    }
  }

  convertGraphRec(super_parent);

  return SettingsUniquePtr(super_parent);
}

void yacl::Config::printConfig(const Setting *sett, uint16_t n_tabs) noexcept {

  const auto print_tabs = [](int tabs) {
    for (uint16_t i = 0; i < tabs; ++i) { std::cout << '\t'; }
  };

  if (!sett->setting_name.empty())
    std::cout << sett->setting_name << " = {\n";
  else
    std::cout << "{\n";

  for (const auto &pair : sett->fields) {
    print_tabs(n_tabs);

    std::cout << pair.first << " = " << pair.second.debugString();
  }

  for (const auto &pair : sett->inner_graph) {
    print_tabs(n_tabs);
    printConfig(pair.second, n_tabs + 1);
  }

  for (const auto &anon : sett->anon_inner_graph) {
    print_tabs(n_tabs);
    printConfig(anon, n_tabs + 1);
  }

  print_tabs(n_tabs - 1);
  std::cout << "}\n";
}

void yacl::Config::convertGraphRec(Setting *graph) noexcept {
  const GraphPair convert_result =
    vectorGraphToMapGraph(graph->inner_graph_vec);
  graph->anon_inner_graph = std::move(convert_result.anon);
  graph->inner_graph = std::move(convert_result.inner);

  for (const auto &f : graph->inner_graph_vec) {
    if (f)
      convertGraphRec(f);
  }
  graph->inner_graph_vec.clear();
}

std::map<std::string, yacl::Field> yacl::Config::vectorFieldsToMapFields(
  std::vector<Field> fields) noexcept {
  std::map<std::string, Field> to_ret;
  for (auto &f : fields) {
    std::string name = f.getName();

    // create var here not necessary
    // but core guidlines told us to always get return value
    const auto it = to_ret.emplace(std::move(name), Field(std::move(f)));
  }

  return to_ret;
}

yacl::Config::GraphPair yacl::Config::vectorGraphToMapGraph(
  const std::vector<Setting*> &fields) noexcept {
  GraphPair result;

  for (const auto &f : fields) {
    if (f->anon)
      result.anon.emplace_back(f);
    else {
      // create var here not necessary
      // but core guidlines told us to always get return value
      const auto it = result.inner.emplace(f->getName(), f);
    }
  }

  return result;
}


std::map<std::string, std::string> yacl::Config::
bracketIntoMap(const std::string &raw_bracket_content) noexcept {

  std::vector<std::string> splitted = smart_bracket_split(raw_bracket_content,
                                                          '=', ';');
  //split(raw_bracket_content, '=');

  std::map<std::string, std::string> result_map;

  if (splitted.size() == 1) { result_map[splitted[0]].clear(); } else {
    for (size_t i = 0; i < splitted.size(); i += 2) {
      result_map[splitted[i]] = splitted[i + 1];
    }
  }

  return result_map;
}

std::vector<yacl::Field> yacl::Config::parseBracketIntoFields(
  const std::string &raw_bracket_content) noexcept {

  std::map<std::string, std::string> bracket_map =
    bracketIntoMap(raw_bracket_content);

  std::vector<Field> fields;
  fields.reserve(bracket_map.size());

  for (const auto &pair : bracket_map) {
    if (isInt(pair.second))
      fields.emplace_back(std::move(Field(pair.first, strToInt(pair.second))));
    else if (isFloat(pair.second))
      fields.emplace_back(std::move(Field(pair.first,
                                          strToFloat(pair.second))));
    else if (isBool(pair.second))
      fields.emplace_back(std::move(Field(pair.first, strToBool(pair.second))));
    else if (isString(pair.second))
      fields.emplace_back(std::move(Field(pair.first, strToStr(pair.second))));
    else if (isArrayOfIntegers(pair.second))
      fields.emplace_back(std::move(Field(pair.first,
                                          raw_to_vector_ints(pair.second))));
    else if (isArrayOfFloats(pair.second))
      fields.emplace_back(std::move(Field(pair.first,
                                          raw_to_vector_floats(pair.second))));
    else if (isArrayOfBools(pair.second))
      fields.emplace_back(std::move(Field(pair.first,
                                          raw_to_vector_bools(pair.second))));
    else if (isArrayOfStrings(pair.second))
      fields.emplace_back(std::move(Field(pair.first,
                                          raw_to_vector_strings(pair.second))));
  }

  return fields;
}
