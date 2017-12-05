import os
EnsureSConsVersion(1, 0, 0)

env = Environment(CPPPATH=Dir('./headerlib').abspath, ENV=os.environ)
env.Replace(CXX=env['ENV']['CXX'] if 'CXX' in env['ENV'] else 'g++')
env.Append(CXXFLAGS='-ftemplate-backtrace-limit=0')
if 'CLANGROOT' in env['ENV']:
    env.Append(CPPFLAGS=env['ENV']['CLANGROOT'].split())

envd = {'env': env}
env.SConscript('test/SConscript', exports=envd)
env.SConscript('elvm.SConscript', exports=envd)
