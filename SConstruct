EnsureSConsVersion(1,0,0)

env = Environment(CPPPATH=Dir('./headerlib').abspath)

envd = {'env':env}
env.SConscript('Permutables/SConscript', exports=envd)
env.SConscript('Bind/SConscript', exports=envd)
env.SConscript('FizzBuzz/SConscript', exports=envd)
env.SConscript('hdrshowcase/SConscript', exports=envd)
