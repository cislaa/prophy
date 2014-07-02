import os

from prophyc import model
from prophyc import model_process

primitive_types = {
    'u8': 'uint8_t',
    'u16': 'uint16_t',
    'u32': 'uint32_t',
    'u64': 'uint64_t',
    'i8': 'int8_t',
    'i16': 'int16_t',
    'i32': 'int32_t',
    'i64': 'int64_t',
    'r32': 'float',
    'r64': 'double',
    'byte': 'uint8_t',
}

def _indent(string_, spaces):
    indentation = spaces * ' '
    return '\n'.join(indentation + x if x else x for x in string_.split('\n'))

def _generate_include(pnodes, include):
    return '#include "{}.pp.hpp"'.format(include.name)

def _generate_constant(pnodes, constant):
    return 'enum {{ {} = {} }};'.format(constant.name, constant.value)

def _generate_typedef(pnodes, typedef):
    tp = primitive_types.get(typedef.type, typedef.type)
    return 'typedef {} {};'.format(tp, typedef.name)

def _generate_enum(pnodes, enum):
    members = ',\n'.join('{} = {}'.format(name, value)
                         for name, value in enum.members)
    return 'enum {}\n{{\n{}\n}};'.format(enum.name, _indent(members, 4))

def _generate_struct(pnodes, struct):
    def gen_member(member):
        def build_annotation(member):
            if member.array_size:
                if member.array_bound:
                    annotation = 'limited array, size in {}'.format(member.array_bound)
                else:
                    annotation = ''
            else:
                if member.array_bound:
                    annotation = 'dynamic array, size in {}'.format(member.array_bound)
                else:
                    annotation = 'greedy array'
            return annotation

        typename = primitive_types.get(member.type, member.type)
        if member.array:
            annotation = build_annotation(member)
            size = member.array_size or 1
            if annotation:
                field = '{0} {1}[{2}]; /// {3}\n'.format(typename, member.name, size, annotation)
            else:
                field = '{0} {1}[{2}];\n'.format(typename, member.name, size)
        else:
            field = '{0} {1};\n'.format(typename, member.name)
        if member.optional:
            return 'prophy::bool_t has_{0};\n'.format(member.name) + field
        return field

    def gen_part(i, part):
        generated = 'struct part{0}\n{{\n{1}}} _{0};'.format(
            i + 2,
            _indent(''.join(map(gen_member, part)), 4)
        )
        return _indent(generated, 4)

    main, parts = pnodes.partition(struct.members)
    generated = _indent(''.join(map(gen_member, main)), 4)
    if parts:
        generated += '\n' + '\n\n'.join(map(gen_part, range(len(parts)), parts)) + '\n'

    return 'struct {}\n{{\n{}}};'.format(struct.name, generated)

def _generate_union(pnodes, union):
    def gen_member(member):
        typename = primitive_types.get(member.type, member.type)
        return '{0} {1};\n'.format(typename, member.name)

    enum_fields = ',\n'.join('discriminator_{0} = {1}'.format(mem.name,
                                                              mem.discriminator)
                             for mem in union.members)
    union_fields = ''.join(map(gen_member, union.members))
    enum_def = 'enum _discriminator\n{{\n{0}\n}} discriminator;'.format(_indent(enum_fields, 4))
    union_def = 'union\n{{\n{0}}};'.format(_indent(union_fields, 4))
    return 'struct {0}\n{{\n{1}\n\n{2}\n}};'.format(union.name,
                                                    _indent(enum_def, 4),
                                                    _indent(union_def, 4))

_generate_visitor = {
    model.Include: _generate_include,
    model.Constant: _generate_constant,
    model.Typedef: _generate_typedef,
    model.Enum: _generate_enum,
    model.Struct: _generate_struct,
    model.Union: _generate_union
}

def _generate(pnodes, node):
    return _generate_visitor[type(node)](pnodes, node)

def _generator(nodes):
    pnodes = model_process.ProcessedNodes(nodes)
    last_node = None
    for node in nodes:
        prepend_newline = bool(last_node
                               and (isinstance(last_node, (model.Enum, model.Struct, model.Union))
                                    or type(last_node) is not type(node)))
        yield prepend_newline * '\n' + _generate(pnodes, node) + '\n'
        last_node = node

header = """\
#ifndef _PROPHY_GENERATED_{0}_HPP
#define _PROPHY_GENERATED_{0}_HPP

#include <prophy/prophy.hpp>
"""

footer = """\
#endif  /* _PROPHY_GENERATED_{0}_HPP */
"""

class CppGenerator(object):

    def __init__(self, output_dir = "."):
        self.output_dir = output_dir

    def generate_definitions(self, nodes):
        return ''.join(_generator(nodes))

    def serialize_string(self, nodes, basename):
        return '\n'.join((
            header.format(basename),
            self.generate_definitions(nodes),
            footer.format(basename)
        ))

    def serialize(self, nodes, basename):
        path = os.path.join(self.output_dir, basename + ".pp.hpp")
        out = self.serialize_string(nodes, basename)
        open(path, "w").write(out)