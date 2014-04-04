# -*- coding: utf-8 -*-

import xml.etree.ElementTree as ElementTree
import model
from itertools import ifilter, islice

"""
TODO:
- arrays of u8 type should be string or bytes fields
"""

def get_include_deps(include):
    return []

def get_constant_deps(constant):
    return filter(lambda x: not x.isdigit(),
                  reduce(lambda x, y: x.replace(y, " "), "()+-", constant.value).split())

def get_typedef_deps(typedef):
    return [typedef.type]

def get_enum_deps(enum):
    return []

def get_struct_deps(struct):
    return [member.type for member in struct.members]

def get_union_deps(union):
    return [member.type for member in union.members]

def get_deps(node):
    return deps_visitor[type(node)](node)

deps_visitor = {model.Include: get_include_deps,
                model.Constant: get_constant_deps,
                model.Typedef: get_typedef_deps,
                model.Enum: get_enum_deps,
                model.Struct: get_struct_deps,
                model.Union: get_union_deps}

def dependency_sort_rotate(nodes, known, available, index):
    node = nodes[index]
    for dep in get_deps(node):
        if dep not in known and dep in available:
            found = next(ifilter(lambda x: x.name == dep, islice(nodes, index + 1, None)))
            found_index = nodes.index(found)
            nodes.insert(index, nodes.pop(found_index))
            return True
    known.add(node.name)
    return False

def dependency_sort(nodes):
    known = set(x + y for x in "uir" for y in ["8", "16", "32", "64"])
    available = set(node.name for node in nodes)

    index = 0
    max_index = len(nodes)

    while index < max_index:
        if not dependency_sort_rotate(nodes, known, available, index):
            index = index + 1

primitive_types = {"8 bit integer unsigned": "u8",
                   "16 bit integer unsigned": "u16",
                   "32 bit integer unsigned": "u32",
                   "64 bit integer unsigned": "u64",
                   "8 bit integer signed": "i8",
                   "16 bit integer signed": "i16",
                   "32 bit integer signed": "i32",
                   "64 bit integer signed": "i64",
                   "32 bit float": "r32",
                   "64 bit float": "r64"}

def make_include(elem):
    return model.Include(elem.get("href").split('.')[0])

def make_constant(elem):
    return model.Constant(elem.get("name"), elem.get("value"))

def make_typedef(elem):
    if "type" in elem.attrib:
        return model.Typedef(elem.get("name"), elem.get("type"))
    elif "primitiveType" in elem.attrib:
        return model.Typedef(elem.get("name"), primitive_types[elem.get("primitiveType")])

def make_enum_member(elem):
    value = elem.get('value')
    value = value if value != "-1" else "0xFFFFFFFF"
    return model.EnumMember(elem.get("name"), value)

def make_enum(elem):
    if len(elem):
        return model.Enum(elem.get("name"), [make_enum_member(member) for member in elem])

def make_struct_members(elem):
    members = []
    ename = elem.get("name")
    etype = elem.get("type")
    dimension = elem.find("dimension")
    if dimension is not None:
        size = dimension.get("size", None)
        if "isVariableSize" in dimension.attrib:
            type = dimension.get("variableSizeFieldType", "u32")
            name = dimension.get("variableSizeFieldName", ename + "_len")
            members.append(model.StructMember(name, type, None, None, None))
            members.append(model.StructMember(ename, etype, True, name, size))
        else:
            members.append(model.StructMember(ename, etype, True, None, size))
    else:
        members.append(model.StructMember(ename, etype, None, None, None))
    return members

def make_struct(elem):
    if len(elem):
        members = reduce(lambda x, y: x + y, (make_struct_members(member) for member in elem))
        return model.Struct(elem.get("name"), members)

def make_union_member(elem):
    return model.UnionMember(elem.get("name"), elem.get("type"), elem.get("discriminatorValue"))

def make_union(elem):
    if len(elem):
        return model.Union(elem.get('name'), [make_union_member(member) for member in elem])

class IsarParser(object):

    def __get_model(self, root):
        nodes = []
        nodes += [make_include(elem) for elem in filter(lambda elem: "include" in elem.tag, root.iterfind('.//*[@href]'))]
        nodes += [make_constant(elem) for elem in root.iterfind('.//constant')]
        nodes += filter(None, (make_typedef(elem) for elem in root.iterfind('.//typedef')))
        nodes += filter(None, (make_enum(elem) for elem in root.iterfind('.//enum')))
        nodes += filter(None, (make_struct(elem) for elem in root.iterfind('.//struct')))
        nodes += filter(None, (make_union(elem) for elem in root.iterfind('.//union')))
        nodes += filter(None, (make_struct(elem) for elem in root.iterfind('.//message')))
        dependency_sort(nodes)
        return nodes

    def parse_string(self, string):
        return self.__get_model(ElementTree.fromstring(string))

    def parse(self, file):
        return self.__get_model(ElementTree.parse(file))