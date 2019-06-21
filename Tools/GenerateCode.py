from collections import OrderedDict
import os
import json

def ParseImportFile(fname):
    mod_dic = OrderedDict()
    with open(fname, 'r') as src:
        for line in src.readlines():
            if line == '' or line == '\n':
                continue
            line = line.rstrip('\n')
            mod_name, lib_name, nid_str = line.split(' ')
            nid = int(nid_str, 16)

            if not mod_name in mod_dic:
                mod_dic[mod_name] = OrderedDict()

            lib_dic = mod_dic[mod_name]
            if not lib_name in lib_dic:
                lib_dic[lib_name] = []

            nid_list = lib_dic[lib_name]
            nid_list.append([nid, 'null'])

    return mod_dic


# some files in ps4libdoc stands for a library
# while most stands for a module
# so we have to fix this
def FixFileName(old_fname):
    new_fname = old_fname
    if old_fname == 'libSceAppContentUtil.sprx.json':
        new_fname = 'libSceAppContent.sprx.json'
    elif old_fname == 'libSceNpScore.sprx.json':
        new_fname = 'libSceNpScoreRanking.sprx.json'
    elif old_fname == 'libSceJson.sprx.json':
        new_fname = 'libSceJson2.sprx.json'

    return new_fname


# https://github.com/idc/ps4libdoc/tree/5.05
def GetFuncNameFromDB(db_root, imp_mods):
    for mod_name in imp_mods:

        print('process module {}'.format(mod_name))
        db_basename = mod_name + '.sprx.json'

        db_basename = FixFileName(db_basename)
        db_fname = os.path.join(db_root, db_basename)
        if not os.path.exists(db_fname):
            print('file not exist {}'.format(db_fname))
            exit(1)

        src = open(db_fname)
        db_obj = json.load(src)
        lib_dic = imp_mods[mod_name]
        db_mod_list = db_obj['modules']
        for db_mod_dic in db_mod_list:
            if db_mod_dic['name'] == mod_name:
                db_lib_list = db_mod_dic['libraries']
                break

        found_lib = False
        for lib_name in lib_dic:
            print('process library {}'.format(lib_name))
            for db_lib_dic in db_lib_list:
                if db_lib_dic['name'] == lib_name:
                    db_sym_list = db_lib_dic['symbols']
                    found_lib = True
                    break

            if not found_lib:
                print('can not find lib {} in {}'.format(lib_name, db_fname))

            func_list = lib_dic[lib_name]


            for func_pair in func_list:
                for db_sym_dic in db_sym_list:
                    db_nid = db_sym_dic['id']
                    if db_nid == func_pair[0]:
                        db_sym_name = db_sym_dic['name']
                        if not db_sym_name:
                            func_name = '_import_{:016X}'.format(func_pair[0])
                        else:
                            func_name = db_sym_name

                        func_pair[1] = func_name
                        break

def SortDic(imp_mods):

    def TakeFunc(elem):
        return elem[1]

    for mod_name in imp_mods:
        lib_dic = imp_mods[mod_name]
        for lib_name in lib_dic:
            func_list = lib_dic[lib_name]
            func_list.sort(key = TakeFunc)

def GetModFolderName(mod_name):
    if len(mod_name) < 6 or mod_name[:6] != 'libSce':
        folder_name = 'Sce' + mod_name.capitalize()
    else:
        folder_name = mod_name[3:]
    return folder_name


def GetCodeFileNames(mod_name):
    if len(mod_name) < 6 or mod_name[:6] != 'libSce':
        base_name = 'sce_' + mod_name.lower()
    else:
        base_name = 'sce_' + mod_name[6:].lower()

    return base_name + '.h', base_name + '.cpp', base_name + '_export.cpp'

def WriteHeadComment(dst, mod_name, lib_dic):
    dst.write('/*' + '\n')
    dst.write(' *' + '    ' + 'GPCS4' + '\n')
    dst.write(' *' + '    ' + '\n')
    dst.write(' *' + '    ' + 'This file implements:' + '\n')
    dst.write(' *' + '    ' + 'module: {}'.format(mod_name) + '\n')
    for lib_name in lib_dic:
        dst.write(' *' + '    ' + '    library: {}'.format(lib_name) + '\n')
    dst.write(' *' + '    ' + '\n')
    dst.write(' */'+ '\n')

def WriteNote(dst):
    dst.write('// Note:' + '\n')
    dst.write('// ' + 'The codebase is generated using {}'.format(os.path.basename(__file__)) + '\n')
    dst.write('// ' + 'You may need to modify the code manually to fit development needs' + '\n')


def FuncNameByNid(nid):
    func_name = '_import_{:016X}'.format(nid)
    return func_name

def WriteOneDecl(dst, func_name, nid):
    if not func_name:
        func_name = FuncNameByNid(nid)

    func_decl = 'int PS4API {}(void);'.format(func_name)
    dst.write(func_decl + '\n')
    dst.write('\n')

def WriteOneImpl(dst, func_name, nid):
    if not func_name:
        func_name = FuncNameByNid(nid)

    func_impl = 'int PS4API {}(void)'.format(func_name)
    dst.write(func_impl + '\n')
    dst.write('{' + '\n')
    dst.write('\tLOG_FIXME("Not implemented");' + '\n')
    dst.write('\treturn SCE_OK;' + '\n')
    dst.write('}' + '\n')
    dst.write('\n')

def WriteDeclaration(dst, func_list):
    for nid, func_name in func_list:
        WriteOneDecl(dst, func_name, nid)
        dst.write('\n')

def WriteDefination(dst, func_list):
    for nid, func_name in func_list:
        WriteOneImpl(dst, func_name, nid)
        dst.write('\n')


def GetExpModuleName(mod_name):
    fmt_mod_name = GetModFolderName(mod_name)
    var_name = 'g_ExpModule{}'.format(fmt_mod_name)
    return var_name


# extern SCE_EXPORT_MODULE g_ExpModuleSceLibc;
def WriteExpTabExtern(dst, mod_name):
    var_name = GetExpModuleName(mod_name)
    dst.write('extern const SCE_EXPORT_MODULE {};\n'.format(var_name))

def GetFuncTableName(mod_name, lib_name):
    func_tab_name = 'g_p{}_{}_FunctionTable'.format(mod_name, lib_name)
    return func_tab_name

def WriteExpTabDefination(dst, mod_name, lib_dic):
    fmt_mod_name = GetModFolderName(mod_name)
    for lib_name in lib_dic:
        func_tab_name = GetFuncTableName(fmt_mod_name, lib_name)
        lib_def_line = 'static const SCE_EXPORT_FUNCTION {}[] =\n'.format(func_tab_name)

        dst.write(lib_def_line)
        dst.write('{\n')

        func_list = lib_dic[lib_name]
        for nid, func in func_list:
            func_ent_line = '\t{{ 0x{0:016X}, "{1:}", (void*){1:} }},\n'.format(nid, func)
            dst.write(func_ent_line)

        dst.write('\tSCE_FUNCTION_ENTRY_END\n')
        dst.write('};\n')
        dst.write('\n')


    lib_ent_name = 'g_p{}_LibTable'.format(fmt_mod_name)
    lib_ent_def = 'static const SCE_EXPORT_LIBRARY {}[] =\n'.format(lib_ent_name)
    dst.write(lib_ent_def)
    dst.write('{\n')

    for lib_name in lib_dic:
        func_tab_name = GetFuncTableName(fmt_mod_name, lib_name)
        lib_ent_line = '\t{{ "{}", {} }},\n'.format(lib_name, func_tab_name)
        dst.write(lib_ent_line)

    dst.write('\tSCE_LIBRARY_ENTRY_END\n')
    dst.write('};\n')
    dst.write('\n')

    mod_def_name = GetExpModuleName(mod_name)
    mod_def_line = 'const SCE_EXPORT_MODULE {} =\n'.format(mod_def_name)
    dst.write(mod_def_line)
    dst.write('{\n')
    dst.write('\t"{}",\n'.format(mod_name))
    dst.write('\t{}\n'.format(lib_ent_name))
    dst.write('};\n')
    dst.write('\n\n')


def WriteInclude(dst, h_name):
    dst.write('#include "{}"'.format(h_name) + '\n')

def WritePragmaOnce(dst):
    dst.write('#pragma once' + '\n')

def WriteLibComment(dst, lib_name):
    dst.write('//////////////////////////////////////////////////////////////////////////' + '\n')
    dst.write('// library: {}'.format(lib_name) + '\n')
    dst.write('//////////////////////////////////////////////////////////////////////////' + '\n')


def WriteModuleFuncDecl(dst):
    dst.write('bool module_start(void* ctx);\n')
    dst.write('void module_exit(void);\n')


def WriteModuleFuncImpl(dst):
    dst.write('bool module_start(void* ctx)\n')
    dst.write('{\n')
    dst.write('\treturn true;\n')
    dst.write('}\n')
    dst.write('\n\n')

    dst.write('void module_exit(void)\n')
    dst.write('{\n')
    dst.write('}\n')


def WriteSourceFiles(fname_h, fname_cpp, fname_exp, mod_name, lib_dic):
    with open(fname_h, 'w') as dst_h, open(fname_cpp, 'w') as dst_cpp, open(fname_exp, 'w') as dst_exp:

        print('write source file {} {} {}'.format(os.path.basename(fname_h), os.path.basename(fname_cpp), os.path.basename(fname_exp)))

        WriteHeadComment(dst_h, mod_name, lib_dic)
        dst_h.write('\n')
        WritePragmaOnce(dst_h)
        dst_h.write('\n')
        WriteInclude(dst_h, 'sce_module_common.h')
        dst_h.write('\n\n')
        WriteExpTabExtern(dst_h, mod_name)
        dst_h.write('\n\n')
        # WriteModuleFuncDecl(dst_h)
        # dst_h.write('\n\n')
        WriteNote(dst_h)
        dst_h.write('\n\n\n')

        WriteInclude(dst_cpp, os.path.basename(fname_h))
        dst_cpp.write('\n\n')
        WriteNote(dst_cpp)
        dst_cpp.write('\n\n\n')

        for lib_name in lib_dic:
            func_list = lib_dic[lib_name]

            WriteLibComment(dst_h, lib_name)
            dst_h.write('\n')
            WriteDeclaration(dst_h, func_list)
            dst_h.write('\n\n')

            WriteLibComment(dst_cpp, lib_name)
            dst_cpp.write('\n')
            WriteDefination(dst_cpp, func_list)
            dst_cpp.write('\n\n')

        # WriteModuleFuncImpl(dst_cpp)

        WriteInclude(dst_exp, os.path.basename(fname_h))
        dst_exp.write('\n\n')
        WriteNote(dst_exp)
        dst_exp.write('\n\n')
        WriteExpTabDefination(dst_exp, mod_name, lib_dic)




def WriteCodeFile(root_dir, imp_mods):

    if not os.path.exists(root_dir):
        os.mkdir(root_dir)

    for mod_name in imp_mods:
        mod_folder = GetModFolderName(mod_name)
        mod_folder = os.path.join(root_dir, mod_folder)
        if not os.path.exists(mod_folder):
            os.mkdir(mod_folder)

        mod_h, mod_cpp, mod_exp_cpp = GetCodeFileNames(mod_name)
        fname_h = os.path.join(mod_folder, mod_h)
        fname_cpp = os.path.join(mod_folder, mod_cpp)
        fname_exp = os.path.join(mod_folder, mod_exp_cpp)

        # if not os.path.exists(fname_h) and not os.path.exists(fname_cpp):
        if True:
            lib_dic = imp_mods[mod_name]
            WriteSourceFiles(fname_h, fname_cpp, fname_exp, mod_name, lib_dic)


def Main():
    # this file is generated by eboot import table, from VS log ouput
    imp_mods = ParseImportFile('import_modules.txt')
    GetFuncNameFromDB('system\\common\\lib', imp_mods)
    SortDic(imp_mods)
    WriteCodeFile('SceModules', imp_mods)
    print('done!')

if __name__ == '__main__':
    Main()