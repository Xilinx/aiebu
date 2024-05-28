#!/usr/bin/env python3
"""
 SPDX-License-Identifier: MIT
 Copyright (c) 2023 Advanced Micro Devices, Inc. All rights reserved
"""

# Features:
#   1. DRC (check if alignment is properly, all operations have size mod 4, etc)
#   2. Generate markdown doc
#   (3. Generate assembler)
#   (4. Generate disassembler)
#   5. Generate parser / stub code for firmware
#   6. Generate C++ library

import sys
import yaml
import markdown
import markdown_graphviz_svg
from jinja2 import Environment, FileSystemLoader


_environment = Environment()
_opsize = 0

def get_arg_name(arg):
  return arg['name'] if 'name' in arg else '-'


def get_arg_width(arg):
  if arg['type'] == 'register' or arg['type'] == 'barrier':
    return 8
  else:
    return arg['width']


def get_arg_c_type(arg):
  width = get_arg_width(arg)
  return 'uint{}_t'.format(width)


def get_operation_size(op):
  size = _opsize
  for arg in op['arguments']:
    size += get_arg_width(arg)
  assert size % 8 == 0
  return size // 8


jinja_functions = {
  'get_arg_name': get_arg_name,
  'get_arg_width': get_arg_width,
  'get_arg_c_type': get_arg_c_type,
  'get_operation_size': get_operation_size
}


def generate_docs(spec):
  template = _environment.get_template('docs.md')
  template.globals.update(jinja_functions)
  print(template.render(spec))


def generate_html_docs(spec):
  template = _environment.get_template('docs.md')
  template.globals.update(jinja_functions)
  md = markdown.Markdown(extensions=[markdown_graphviz_svg.GraphvizBlocksExtension(), 'tables', 'fenced_code', 'toc'])
  html = md.convert(template.render(spec))

  html_template = _environment.get_template('docs.html')
  print(html_template.render({
    'html': html,
    'toc': md.toc
  }))


def generate_cpp_assembler_stubs(spec):
  template = _environment.get_template('assembler_stub.h')
  template.globals.update(jinja_functions)
  print(template.render(spec))

def generate_c_stubs(spec):
  template = _environment.get_template('stubs.h')
  template.globals.update(jinja_functions)
  print(template.render(spec))


def generate_py(spec):
  template = _environment.get_template('stubs.py')
  template.globals.update(jinja_functions)
  print(template.render(spec))


def postprocess_spec(spec):
  global _opsize
  platform = spec['platform']
  _opsize = platform['opsize']

  for op in spec['operations']:
    offset = _opsize
    for arg in op['arguments']:
      arg['_offset'] = offset // 8
      offset += get_arg_width(arg)


def run(spec_filename, template_dir, command):
  spec = None

  global _environment
  _environment = Environment(loader=FileSystemLoader(template_dir))

  with open(spec_filename, 'r') as f:
    spec = yaml.load(f, Loader=yaml.Loader)
  postprocess_spec(spec)

  if command == 'generate_docs':
    generate_docs(spec)
  elif command == 'generate_html_docs':
    generate_html_docs(spec)
  elif command == 'generate_c_stubs':
    generate_c_stubs(spec)
  elif command == 'generate_cpp_assembler_stubs':
    generate_cpp_assembler_stubs(spec)
  elif command == 'generate_py':
    generate_py(spec)
  else:
    sys.stderr.write("Invalid command: {}\n".format(command))
    sys.exit(1)


if __name__ == '__main__':
  if len(sys.argv) != 4:
    sys.stderr.write("Usage: {} <spec-filename> <template-dir> <command>\n".format(
      sys.argv[0]
    ))
    sys.exit(1)

  run(sys.argv[1], sys.argv[2], sys.argv[3])
