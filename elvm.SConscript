import os
Import('env')

vardir = env.VariantDir('build/elvm', 'elvm')
env.Command(
    target=['build/elvm/out/elc'],
    source=[
        env.Glob('build/elvm/ir/*.c'), env.Glob('build/elvm/ir/*.h'),
        env.Glob('build/elvm/target/*.c'), env.Glob('build/elvm/target/*.h'),
        'build/elvm/Makefile', env.Glob('build/elvm/*.mk'),
        env.Glob('build/elvm/tools/*')],
    action=Action(lambda target, source, env: os.system('make out/elc')),
    chdir='build/elvm')
