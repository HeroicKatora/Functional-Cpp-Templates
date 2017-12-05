import os
Import('env')

vardir = env.VariantDir('build/elvm', 'elvm')
env.Command(
	target=['build/elvm/Makefile'],
	source='elvm/Makefile',
	action='sed -e "/.git/ { s/:.*/:/ }" $SOURCE > $TARGET')
env.Command(
    target=['build/elvm/out/elc', 'build/elvm/out/8cc'],
    source=[
        env.Glob('build/elvm/ir/*.c'), env.Glob('build/elvm/ir/*.h'),
        env.Glob('build/elvm/target/*.c'), env.Glob('build/elvm/target/*.h'),
		env.Glob('build/elvm/8cc/*.c'), env.Glob('build/elvm/8cc/*.h'), 'build/elvm/8cc/Makefile',
        env.Glob('build/elvm/*.mk'),
        env.Glob('build/elvm/tools/*')],
    action='make out/elc out/8cc out/eli',
    chdir='build/elvm')
