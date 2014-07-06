import os
import sys
import argparse

def readable_dir(string):
    if not os.path.isdir(string):
        raise argparse.ArgumentTypeError("%s directory not found" % string)
    return string

def readable_file(string):
    if not os.path.isfile(string):
        raise argparse.ArgumentTypeError("%s file not found" % string)
    return string

def parse_options():
    class ArgumentParser(argparse.ArgumentParser):
        def error(self, message):
            self.exit(1, '%s: error: %s\n' % (self.prog, message))

    parser = ArgumentParser('prophyc',
                            description = ('Parse input files and generate '
                                           'output based on options given.'))

    parser.add_argument('input_files',
                        metavar = 'INPUT_FILE',
                        type = readable_file,
                        nargs = '+',
                        help = ('C++ or isar xml files with definitions of prophy '
                                'messages.'))

    group = parser.add_mutually_exclusive_group(required = True)
    group.add_argument('--isar',
                       action = 'store_true',
                       help = 'Parse input files as isar xml.')
    group.add_argument('--sack',
                       action = 'store_true',
                       help = 'Parse input files as sack C++.')

    parser.add_argument('-I', '--include_dir',
                        metavar = 'DIR',
                        dest = 'include_dirs',
                        type = readable_dir,
                        action = 'append',
                        default = [],
                        help = ('Specify the directory in which to search for '
                                'include directories in sack mode.  '
                                'May be specified multiple times.'))

    parser.add_argument('-p', '--patch',
                        metavar = 'FILE',
                        type = readable_file,
                        help = ("File with instructions changing definitions of prophy "
                                "messages after parsing. It's needed in sack and isar "
                                "modes, since C++ and isar xml they're unable to express "
                                "all prophy features."))

    parser.add_argument('--python_out',
                        metavar = 'OUT_DIR',
                        type = readable_dir,
                        help = 'Generate Python source file.')

    parser.add_argument('--cpp_out',
                        metavar = 'OUT_DIR',
                        type = readable_dir,
                        help = 'Generate C++ header and source files.')

    return parser.parse_args()
