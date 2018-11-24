# Into
Write stdin to an output file, but only if that would change its contents.

    Usage: into <output_file>

Reads data up to EOF from stdin, compares it to the contents of `output_file` and if it is different writes it out to that file.

Written to function as the end of a pipeline that generates dependencies for a source file. If you write to a dependency file every time you generate it, the make program would see a new modification time and think that it's changed every time. That means all targets that depend on it are also made anew.

If, instead, you do something like:

    gcc -MM foo.c | into foo.d

then `foo.d` only gets a new modification time if it actually changes, not every time this command is executed. Which means any downstream commands are also only executed if necessary.

Note that all of stdin is read into memory before it is compared to `output_file`, so this should probably not be used on gigabytes of input. 
