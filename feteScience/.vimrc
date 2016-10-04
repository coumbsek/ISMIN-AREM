set nocompatible      " Nécessaire
filetype off          " Nécessaire


" Ajout de Vundle au runtime path et initialisation
 set rtp+=~/.vim/bundle/Vundle.vim
 call vundle#begin()
     
" On indique à Vundle de s'auto-gérer :)
Plugin 'gmarik/Vundle.vim'  " Nécessaire
Plugin 'scrooloose/nerdtree.git'
"Plugin 'mbbill/code_complete.git'
"Bundle 'Valloric/YouCompleteMe'
Bundle 'rdnetto/YCM-Generator'
" C'est ici que vous allez placer la liste des plugins que Vundle doit gérer

call vundle#end()            " Nécessaire
filetype plugin indent on    " Nécessaire

let g:ycm_global_ycm_extra_conf = "~/.vim/.ycm_extra_conf.py"   

set nu
syntax on
filetype plugin indent on
set backspace=indent,eol,start
set ruler
set showmode
set mouse=a

map <C-t> :NERDTreeToggle<CR>

"inoremap ( ()<Left>
"inoremap [ []<Left>
"inoremap { {<Return><Return><Backspace>}<Up><Tab>

inoremap #bi #include <stdio.h><Return>#include <stdlib.h><Return><Return>
inoremap #main int<Tab>main(int argc, char **argv){<Return><Return><Return><Backspace>}<Up><Tab>exit(EXIT_SUCCESS);
inoremap printf printf("\n");<Left><Left><Left><Left><Left>
