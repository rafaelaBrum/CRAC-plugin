import re
import copy

from jinja2 import Template, Environment, FileSystemLoader

jinja_env = Environment(trim_blocks=True, loader=FileSystemLoader('.'))

opengl_wrappers_template = jinja_env.get_template('opengl_wrappers_template.cpp')
opengl_replay_template = jinja_env.get_template('opengl_replay_template.cpp')

opengl_functions = []

line = ''
class FunctionInfo:
    def __init__(self):
        self.log = False
        self.name = None
        # values in this dict are probably out of order. Use the pos
        # field of the args to sort.
        self.args = {}
        self.return_type = None

    def get_arg(self, arg_name):
        if arg_name not in self.args:
            self.args[arg_name] = FunctionArg(name=arg_name)
        return self.args[arg_name]

    def get_args(self):
        return sorted(self.args.values(), key=lambda x: x.pos)

    def add_arg(self, arg_type, arg_name, pos):
        if arg_name not in self.args:
            self.args[arg_name] = FunctionArg(type_=arg_type, name=arg_name, pos=pos)
        else:
            self.args[arg_name].type_ = arg_type
            assert(self.args[arg_name].name == arg_name)
            self.args[arg_name].pos = pos

    def args_signature(self):
        return ', '.join(arg.get_declaration() for arg in self.get_args())

    def arg_names(self):
        return [arg.name for arg in self.get_args()]

    def arg_names_commas(self):
        return ', '.join(self.arg_names())


class FunctionArg:
    def __init__(self, type_=None, name=None, pos=-1):
        self.type_ = type_
        self.name = name
        self.flags = {}
        self.pos = pos

    def get_declaration(self):
        return f'{self.type_} {self.name}'

    def is_ptr(self):
        return '*' in self.type_

    def copy_as_buf(self):
        return self.is_ptr() and 'buf_length' in self.flags

    def buf_len(self):
        if 'buf_length' not in self.flags:
            assert False
        return self.flags['buf_length']

    def deref_type(self):
        assert self.type_[-1] == '*' # lol
        return self.type_[:-1]


function_info = FunctionInfo()
functions = []
for line_ in open('opengl_wrapper_signatures.txt'):
    line_ = line_.strip()
    if not line_:
        continue

    if line_.split()[0] == '//':
        continue
    elif line_.split()[0] == '///':
        line_ = line_[3:].strip()
        if not line_:
            continue
        decorator_argv = line_.split()
        if decorator_argv[0] == 'log':
            function_info.log = True
            print('Log this function.')
        elif decorator_argv[0] == 'arg':
            arg_name = decorator_argv[1]
            arg_param = decorator_argv[2]
            arg_param_value = decorator_argv[3]
            function_info.get_arg(arg_name).flags[arg_param] = arg_param_value
            print(f'Found arg param for f{arg_name}: arg_param arg_param_value')
        else:
            print(f'Unrecognized decorator: f{line}')
            assert False
        continue

    line += line_
    if ';' not in line_:
        continue

    parse_matches = re.fullmatch(r'^(.*[ *]|^)([a-zA-Z0-9_]+)\((.*)\);$', line)
    if not parse_matches:
        print(f"cannot parse declaration {line}")
        continue
    return_type = parse_matches.group(1)
    return_type = return_type.strip()
    if not return_type.strip():
        return_type = 'void'
    print(f'Return type: {return_type}')
    function_info.return_type = return_type

    func_name = parse_matches.group(2)
    func_name = func_name.strip()
    print(f'Function name: {func_name}')
    function_info.name = func_name

    args_sig = parse_matches.group(3)
    args_sig_split = args_sig.split(',')
    args_sig_split = [arg.strip() for arg in args_sig_split]
    arg_names = []

    for pos, arg_sig in enumerate(args_sig_split):
        if not arg_sig:
            continue
        arg_name_matches = re.fullmatch(r'^(.*[ *])([a-zA-Z0-9_]+)$', arg_sig)
        if not arg_name_matches or len(arg_name_matches.groups()) != 2:
            print(f'cannot parse argument `{arg_sig}`')
            continue
        arg_type = arg_name_matches.group(1).strip()
        arg_name = arg_name_matches.group(2).strip()
        print(f'Found arg with type: {arg_type}, name: {arg_name}')
        function_info.add_arg(arg_type, arg_name, pos)
        arg_names.append(arg_name)

    functions.append(function_info)
    line = ''
    function_info = FunctionInfo()

f = open('opengl_wrappers.cpp', 'w')
f.write(opengl_wrappers_template.render(funcs=functions))
f.close()

f = open('opengl_replay.cpp', 'w')
f.write(opengl_replay_template.render(funcs=functions))
f.close()

lower_half_opengl_if_template = jinja_env.get_template('lower_half_opengl_if_template.h')
open('lower_half_opengl_if.h', 'w').write(lower_half_opengl_if_template.render(func_names=[v.name for v in functions]))


