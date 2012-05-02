#ifndef ZION_MODULE_H
#define ZION_MODULE_H

typedef int (*init_t)(void);
typedef void (*exit_t)(void);

extern init_t __init_start;
extern init_t __init_end;
extern exit_t __exit_start;
extern exit_t __exit_end;

#define __used		__attribute__((__used__))
#define __section(S)	__attribute__((__section__(#S)))

#define __define_initcall(level,fn,id) \
	static init_t __init_##fn##id __used \
	__section(.module##id.init) = fn;

#define __define_exitcall(level,fn,id) \
	static exit_t __exit_##fn##id __used \
	__section(.module#id.exit) = fn;

#define arch_init(fn)		__define_initcall("0",fn,0)
#define core_init(fn)		__define_initcall("1",fn,1)
#define module_init(fn)		__define_initcall("2",fn,2)

#define arch_exit(fn)		__define_exitcall("0",fn,0)
#define core_exit(fn)		__define_exitcall("1",fn,1)
#define module_exit(fn)		__define_exitcall("2",fn,2)

#endif
