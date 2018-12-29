##runtimepath
runtimepath alias rtp 
										Unix: "$HOME/.vim,
                                                $VIM/vimfiles,
                                                $VIMRUNTIME,
                                                $VIM/vimfiles/after,
                                                $HOME/.vim/after"

This is a list of directories which will be searched for runtime
        files:
          filetype.vim  filetypes by file name new-filetype
          scripts.vim   filetypes by file contents new-filetype-scripts
          autoload/     automatically loaded scripts autoload-functions
          colors/       color scheme files :colorscheme
          compiler/     compiler files :compiler
          doc/          documentation write-local-help
          ftplugin/     filetype plugins write-filetype-plugin
          indent/       indent scripts indent-expression
          keymap/       key mapping files mbyte-keymap
          lang/         menu translations :menutrans
          menu.vim      GUI menus menu.vim
          plugin/       plugin scripts write-plugin
          print/        files for printing postscript-print-encoding
          spell/        spell checking files spell
          syntax/       syntax files mysyntaxfile
          tutor/        files for vimtutor tutor
**我们可以把rtp理解为vim的搜索路径**
##autoload 
automatically loaded scripts autoload-functions
Using a script in the "autoload" directory is simpler, but requires using
exactly the right file name.  A function that can be autoloaded has a name
like this:

        :call filename#funcname()

When such a function is called, and it is not defined yet, Vim will search the
"autoload" directories in 'runtimepath' for a script file called
"filename.vim".  For example "~/.vim/autoload/filename.vim".  That file should
then define the function like this:

        function filename#funcname()
           echo "Done!"
        endfunction

The file name and the name used before the # in the function must match
exactly, and the defined function must have the name exactly as it will be
called.

It is possible to use subdirectories.  Every # in the function name works like
a path separator.  Thus when calling a function:

        :call foo#bar#func()

Vim will look for the file "autoload/foo/bar.vim" in 'runtimepath'.

This also works when reading a variable that has not been set yet:

        :let l = foo#bar#lvar
**autoload  和 rpt有关，autoload机制会去rtp路径下的autoload目录寻找对应的文件和函数**

##autocmd
:au[tocmd] [group] {event} {pat} [nested] {cmd}
                        Add {cmd} to the list of commands that Vim will
                        execute automatically on {event} for a file matching
                        {pat} autocmd-patterns.
                        Vim always adds the {cmd} after existing autocommands,
                        so that the autocommands execute in the order in which
                        they were given.  See autocmd-nested for [nested].
**简单来讲，autocmd是类似linux系统中的PATH变量，它定义了一些自动执行的命令集合，会在vim启动的时候执行**

##buf*
###bufEnter
After entering a buffer.  Useful for setting
                                options for a file type.  Also executed when
                                starting to edit a buffer, after the
                                BufReadPost autocommands.

	autocmd BufEnter * lcd %:p:h

上面这个命令，lcd就是进入的意思，%当前文件名，`:p`表示全名，`:p`表示去掉文件名。这个命令的意思是进入对于当前编辑的文件，将pwd切换到文件所在目录。

##filetype
                                                        :filetype :filet
To enable file type detection, use this command in your vimrc:

        :filetype on

Each time a new or existing file is edited, Vim will try to recognize the type
of the file and set the 'filetype' option.  This will trigger the FileType
event, which can be used to set the syntax highlighting, set options, etc.

NOTE: Filetypes and 'compatible' don't work together well, since being Vi
compatible means options are global.  Resetting 'compatible' is recommended,
if you didn't do that already.

###:filetype-plugin-on
You can enable loading the plugin files for specific file types with:

        :filetype plugin on

If filetype detection was not switched on yet, it will be as well.
This actually loads the file `ftplugin.vim` in `runtimepath`.
The result is that when a file is edited its plugin file is loaded (if there
is one for the detected filetype). filetype-plugin

###:filetype-indent-on
You can enable loading the indent file for specific file types with:

        :filetype indent on

If filetype detection was not switched on yet, it will be as well.
This actually loads the file `indent.vim` in `runtimepath`.
The result is that when a file is edited its indent file is loaded (if there
is one for the detected filetype). indent-expression

**注意：autoindent 的原理是copy一行的indent**
##windows
###popup menu

pumvisible()                                            pumvisible()
                Returns non-zero when the popup menu is visible, zero
                otherwise.  See ins-completion-menu.
                This can be used to avoid some things that would remove the
                popup menu.

popup menu是一个提示菜单，vim可以在自动补全的时候插入提示菜单，这需要调用ins-completion-menu.

##let
:let {var-name} = {expr1}                               *:let* *E18*
                        Set internal variable {var-name} to the result of the
                        expression {expr1}.  The variable will get the type
                        from the {expr}.  If {var-name} didn't exist yet, it
                        is created.
## other

###silent
 避免出现提示信息；

###plug                                                         <Plug>

The special key name "<Plug>" can be used for an internal mapping, which is
not to be matched with any key sequence.  This is useful in plugins
using-<Plug>.

##recording
q{0-9a-zA-Z"}           Record typed characters into register {0-9a-zA-Z"}
                        (uppercase to append).  The 'q' command is disabled
                        while executing a register, and it doesn't work inside
                        a mapping.  {Vi: no recording}

q                       Stops recording.  (Implementation note: The 'q' that
                        stops recording is not stored in the register, unless
                        it was the result of a mapping)  {Vi: no recording}
