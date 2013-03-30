µcblog
================================================================================

`µcblog` is a C based static website generator. It is inspired by
[jekyll](https://github.com/mojombo/jekyll) and [growl](https://github.com/xfire/growl)
and aims to be very simple to use, with very few dependencies. And because I'm
an embedded engineer, it's meant to be used on embedded systems and for self-hosting.
Just Linux and ucblog ! No other boring stuff to install like jekyll.

**03-2013 Note**: This project is not maintained anymore. Two reasons :

- libctemplate is old and not so adapted to µcblog usage. I would need to
develop a new light template library in C. Like jinja2 in Python. But, this is
to much work for myself, and not so fun. Moreover I'd prefer to use another
license, like MIT.
- I switched to [growl](https://github.com/yoannsculo/growl) for my personal use

So, the project stays here for posterity :P Don't pay attention to the
quality/architecture of the project. It was a Proof of Concept at first and I
never took the time to switch to something more organised and clean.

Features
--------------------------------------------------------------------------------
- developed in C, very small binary with few dependencies
- your website architecture is based on the filesystem. Aimed to be as natural
as possible for users.
- markdown formatted data as input - Using [Sundown](https://github.com/yoannsculo/sundown) (MIT)
- Html templating - Using [libctemplate](http://libctemplate.sourceforge.net) (GPL)
and something like [liquid](https://github.com/Shopify/liquid) or [jinja2](http://jinja.pocoo.org/docs/)
that doesn't exist in C yet.
- HTML/CSS/JS rendering with [bootstrap](https://github.com/twitter/bootstrap) and
[google-code-prettify](http://code.google.com/p/google-code-prettify/) to
make your blog prettier ;) But you can use whatever HTML/CSS/JS you want.

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

Licensed under the terms of the [GPL License](http://www.gnu.org/licenses/gpl.html)

Other :
 - libhtmltpl	: http://pavluk.org/projects/libcgic/libhtmltpl/index.html

Contact
-------

Yoann Sculo - <yoann.sculo@gmail.com>
