# Tracker

Tracker is an RDF (resource data format) storage engine. RDF consists of
*triplets* such as subject:verb:object. For example, there could be
standard triplets for book titles and authors, such as

    Othello:has-author:William Shakespeare

. A standardized set of triplet forms is called an *ontology*.

Tracker provides a storage for such triplets, and a query engine in the
form of the SPARQL query language.

GNOME uses Tracker as a storage for document metadata. A document's
metadata may include its title, authors, copyright, modification date,
and keywords. All this metadata is stored as RDF triplets in Tracker,
and queried with SPARQL by applications such as Gnome Documents. The
[ontology used](http://developer.gnome.org/ontology/unstable/) is based
on various standard sub-ontologies: [Dublin
Core](http://dublincore.org/) for document metadata, and
[NEPOMUK](http://nepomuk.kde.org/) for annotations, files, contacts and
other items.

  - [Tracker Home Page](https://wiki.gnome.org/Projects/Tracker)

  - [Tracker
    documentation](https://wiki.gnome.org/Projects/Tracker/Documentation)
