import re

from jinja2 import Template, Environment, FileSystemLoader

jinja_env = Environment(trim_blocks=True, loader=FileSystemLoader('.'))

opengl_wrappers_template = jinja_env.get_template('opengl_wrappers_template.cpp')

opengl_functions = []

line = ''
for line_ in open('opengl_wrapper_signatures.txt'):
    line_ = line_.strip()
    if not line_:
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
    func_name = parse_matches.group(2)
    func_name = func_name.strip()
    print(f'Function name: {func_name}')
    args_sig = parse_matches.group(3)
    args_sig_split = args_sig.split(',')
    args_sig_split = [arg.strip() for arg in args_sig_split]
    arg_names = []

    for arg_sig in args_sig_split:
        if not arg_sig:
            continue
        arg_name_matches = re.fullmatch(r'^(.*[ *])([a-zA-Z0-9_]+)$', arg_sig)
        if not arg_name_matches or len(arg_name_matches.groups()) != 2:
            print(f'cannot parse argument `{arg_sig}`')
            continue
        arg_type = arg_name_matches.group(1).strip()
        arg_name = arg_name_matches.group(2).strip()
        print(f'Found arg with type: {arg_type}, name: {arg_name}')
        arg_names.append(arg_name)

    opengl_functions.append({
        'return_type': return_type,
        'name': func_name,
        'args_sig': args_sig,
        'args': ', '.join(arg_names),
        'arg_name_list': arg_names,
    })

    line = ''

f = open('opengl_wrappers.cpp', 'w')
f.write(opengl_wrappers_template.render(opengl_funcs=opengl_functions))
f.close()

lower_half_opengl_if_template = jinja_env.get_template('lower_half_opengl_if_template.h')
open('lower_half_opengl_if.h', 'w').write(lower_half_opengl_if_template.render(func_names=[v['name'] for v in opengl_functions]))


