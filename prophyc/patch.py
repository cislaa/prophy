from collections import namedtuple
from . import model
from collections import OrderedDict

Action = namedtuple("Action", ["action", "params"])

def parse(filename):
    def make_item(line):
        words = line.split()
        name, action = words[:2]
        params = words[2:]
        return name, Action(action, params)
    patches = OrderedDict()
    for name, action in (make_item(line) for line in open(filename) if line.strip()):
        patches.setdefault(name, []).append(action)
    return patches

def find_node(nodes, name):
    return next((x for x in enumerate(nodes) if x[1].name == name), (None, None))

def find_member(node, name):
    return find_node(node.members, name)

def patch(nodes, patches):
    for patch in patches:
        nodes_to_patch = [ x for x in nodes if x.name == patch]
        for node in nodes_to_patch:
            actions = patches[patch]
            for action in actions:
                _apply(node, action, nodes)

def _apply(node, patch, nodes):
    action = _actions.get(patch.action)
    if not action:
        raise Exception("Unknown action: %s %s" % (node.name, patch))
    action(node, patch, nodes)

def _type(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can change field only in struct: %s %s" % (node.name, patch))

    if len(patch.params) != 2:
        raise Exception("Change field must have 2 params: %s %s" % (node.name, patch))
    name, tp = patch.params

    i, member = find_member(node, name)
    if not member:
        raise Exception("Member not found: %s %s" % (node.name, patch))

    mem = node.members[i]
    mem.type = tp

def _insert(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can insert field only in struct: %s %s" % (node.name, patch))

    if len(patch.params) != 3:
        raise Exception("Change field must have 3 params: %s %s" % (node.name, patch))
    index, name, tp = patch.params

    if not _is_int(index):
        raise Exception("Index is not a number: %s %s" % (node.name, patch))
    index = int(index)

    node.members.insert(index, model.StructMember(name, tp))

def _remove(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can remove field only in struct: %s %s" % (node.name, patch))

    if len(patch.params) != 1:
        raise Exception("Remove field must have 1 param: %s %s" % (node.name, patch))
    name, = patch.params

    i, member = find_member(node, name)
    if not member:
        raise Exception("Member not found: %s %s" % (node.name, patch))

    del node.members[i]

def _dynamic(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can change field only in struct: %s %s" % (node.name, patch))

    if len(patch.params) != 2:
        raise Exception("Change field must have 2 params: %s %s" % (node.name, patch))
    name, len_name = patch.params

    i, member = find_member(node, name)
    if not member:
        raise Exception("Member not found: %s %s" % (node.name, patch))

    mem = node.members[i]
    mem.array = True
    mem.bound = len_name
    mem.size = None
    mem.optional = False

def _greedy(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can change field only in struct: %s %s" % (node.name, patch))

    if len(patch.params) != 1:
        raise Exception("Change field must have 1 params: %s %s" % (node.name, patch))
    name, = patch.params

    i, member = find_member(node, name)
    if not member:
        raise Exception("Member not found: %s %s" % (node.name, patch))

    mem = node.members[i]
    mem.array = True
    mem.bound = None
    mem.size = None
    mem.optional = False

def _static(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can change field only in struct: %s %s" % (node.name, patch))

    if len(patch.params) != 2:
        raise Exception("Change field must have 2 params: %s %s" % (node.name, patch))
    name, size = patch.params

    if not _is_int(size):
        raise Exception("Size is not a number: %s %s" % (node.name, patch))

    i, member = find_member(node, name)
    if not member:
        raise Exception("Member not found: %s %s" % (node.name, patch))

    node.members[i].bound = None
    node.members[i].size = None

    mem = node.members[i]
    mem.array = True
    mem.bound = None
    mem.size = size
    mem.optional = False

def _limited(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can change field only in struct: %s %s" % (node.name, patch))

    if len(patch.params) != 2:
        raise Exception("Change field must have 2 params: %s %s" % (node.name, patch))
    name, len_array = patch.params

    i, member = find_member(node, len_array)
    if not member:
        raise Exception("Array len member not found: %s %s" % (node.name, patch))

    i, member = find_member(node, name)
    if not member:
        raise Exception("Member not found: %s %s" % (node.name, patch))

    mem = node.members[i]
    mem.array = True
    mem.bound = len_array
    mem.optional = False

def _discriminated(node, patch, nodes):
    if not isinstance(node, model.Struct):
        raise Exception("Can change field only in struct: %s %s - type %s" % (node.name, patch, type(node)))

    if len(patch.params) != 2:
        raise Exception("Change field must have 2 params: %s %s" % (node.name, patch))

    union, discriminator = patch.params
    _, member = find_member(node, union)
    if not member:
        raise Exception("Member \"%s\" not found: %s, %s" % (union, node.name, patch))

    _, union_declaration = find_node(nodes, member.type)
    if not union_declaration:
        raise Exception("Union type not found: %s %s" % (member.type, patch))

    _, member = find_member(node, discriminator)
    if not member:
        raise Exception("Member \"%s\" not found: %s, %s" % (discriminator, node.name, patch))

    _, enum_declaration = find_node(nodes, member.type)
    if not enum_declaration:
        raise Exception("Discriminator type not found: %s %s" % (member.type, patch))

    new_node = model.Union(node.name, [model.UnionMember(x.name, x.type, y.value, definition = x.definition) \
                                   for x, y in zip(union_declaration.members, enum_declaration.members)])
    new_node.discriminator = enum_declaration

    i, _ = next((x for x in enumerate(nodes) if x[1].name == node.name), (None, None))
    nodes.remove(node)
    nodes.insert(i, new_node)

_actions = {'type': _type,
            'insert': _insert,
            'remove': _remove,
            'greedy': _greedy,
            'static': _static,
            'limited': _limited,
            'dynamic': _dynamic,
            'discriminated': _discriminated}

def _is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False
