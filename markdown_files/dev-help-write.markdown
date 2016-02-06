# Write some help

## Set up help

[]()

Write some [Mallard](http://www.projectmallard.org/) pages and add them
to .

Most projects should have an and some content pages, although you may
not need an if you are dropping plugin help into another project.

> 
> 
> ``` 
>       help/C/index.page
> 
> <page xmlns="http://projectmallard.org/1.0/"
>       xmlns:its="http://www.w3.org/2005/11/its"
>       type="guide"
>       id="index">
> 
>   <info>
>     <revision pkgversion="3.9" date="2013-06-19" status="stub"/>
> 
>     <include href="legal.xml" xmlns="http://www.w3.org/2001/XInclude"/>
>   </info>
> 
>   <title>
>     <media type="image" mime="image/png" its:translate="no" src="figures/icon.png" />
>     Application name
>   </title>
> 
>   <section id="features" style="2column">
>     <title>Features</title>
>   </section>
> 
> </page>
> 
>     
> ```
> 
> ``` 
>       help/C/introduction.page
> 
> <page xmlns="http://projectmallard.org/1.0/"
>       xmlns:its="http://www.w3.org/2005/11/its"
>       type="topic"
>       id="introduction">
> 
>   <info>
>     <link type="guide" xref="index"/>
> <!--
>     <link type="guide" xref="index#features"/>
>     <link type="seealso" xref="anotherpageid"/>
> -->
>     <revision pkgversion="3.9" date="2013-06-19" status="stub"/>
>  
>     <credit type="author">
>       <name>Your Name</name>
>       <email its:translate="no">mail@example.com</email>
>       <years>2013</years>
>     </credit>
>  
>     <include href="legal.xml" xmlns="http://www.w3.org/2001/XInclude"/>
> 
>     <desc>Welcome to _Application name_</desc>
>   </info>
> 
>   <title>Introduction</title>
> 
>   <p>This application is awesome!</p>
> 
> </page>
> 
>     
> ```
