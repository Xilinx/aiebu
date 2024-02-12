#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include "aiebu_assembler.h"
#include "aiebu_error.h"
#include <algorithm>

using namespace std;

int main(int argc, char ** argv)
{
  std::ifstream input( argv[1], std::ios::binary );
  vector<char> v;
  std::copy(std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>( ),
            std::back_inserter(v));

  std::vector<aiebu::patch_info> patch_data;
  try
  {
    auto as = aiebu::aiebu_assembler(aiebu::aiebu_assembler::buffer_type::asm_aie2ps, v);
    auto e = as.get_elf();
    cout << "elf size:" << e.size() << "\n";
    std::ofstream output_file("out.elf");
    std::ostream_iterator<char> output_iterator(output_file);
    std::copy(e.begin(), e.end(), output_iterator);
  }
  catch (aiebu::error &ex)
  {
    std::cout << "ERROR: " <<  ex.what() << ex.get_code() << std::endl;
  }


  return 0;
}
