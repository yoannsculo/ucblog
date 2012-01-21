µcblog
================================
µcblog is a c based static website generator.

Using :

 - sundown	: https://github.com/tanoku/sundown
 - libctemplate : http://libctemplate.sourceforge.net (GPLV3)

The project is in progress and subject to big changes. Full documentation will follow, as soon as I get something fully working.


Other :
 - libhtmltpl	: http://pavluk.org/projects/libcgic/libhtmltpl/index.html
 - nftw

layout :

main.tpl includes :
  content/blog.tpl
  content/page.tpl
  content/book.tpl


Engine :

- parsing ./source/depth_0 {blog, pages} dir
	- moving ./source/images to ./dest/images
	- moving ./source/documents to ./dest/documents
	- transforming ./source/mkd to ./dest/html content

- parsing ./dest
	- generating html file from html content applying templates

i.e.

<tmpl_var name="content_main">

	<tmpl_var name="content_blog">

