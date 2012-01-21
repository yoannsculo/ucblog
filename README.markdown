µcblog
================================================================================
`µcblog` is a C based static website generator. It is inspired by jekyll and
growl and aims to be very simple to use, with very few dependencies.
And because I'm an embedded engineer, it's meant to be used on embedded systems
and for self-hosting. Just Linux and ucblog ! No other boring stuff to install
like jekyll.

The project is in progress and subject to big changes. Full documentation will
follow, as soon as I get something fully working.

Features
--------------------------------------------------------------------------------
- developed in C, very small binary with few dependencies
- your website architecture is based on the filesystem. Aimed to be as natural
as possible for users.
- markdown formatted data as input - Using [Sundown](https://github.com/tanoku/sundown)
- Html templating - Using [libctemplate](http://libctemplate.sourceforge.net)
and something like [liquid](https://github.com/Shopify/liquid) or [jinja2](http://jinja.pocoo.org/docs/)
that doesn't exist in C yet.

installation
--------------------------------------------------------------------------------
	> cd ucblog
	> make

How simple is that ?

(I will tweak the makefile to get something more advanced, but later)

Usage
--------------------------------------------------------------------------------
	> ./ucblog ./sources ./tmp

License
--------------------------------------------------------------------------------
Not yet decided which GPL License I will use.

Other :
 - libhtmltpl	: http://pavluk.org/projects/libcgic/libhtmltpl/index.html
