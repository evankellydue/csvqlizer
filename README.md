# CSVQLIZER

**Disclaimer: I am on the week 4 problem set of Harvard's CS50 course 2025 and realized I could create this useful thing for myself.**

This is a very rough project that aims to accomplish a common work task:

Given a CSV export from a random records management system, how can I rename the columns to be dbeaver sql-autocomplete compatible and create a table to import to where the data type is varchar?

#### Why varchar?

I find editing/transforming/analyzing CSVs to be easier in postgres than in excel, because I can write whatever logic I want. I also prefer having empty values as null (as opposed to making everything text)
It is a lot easier for me to treat everything as a string and do operations like `split_part`, `concat_ws`, and `ilike '%text%'` and transform the data into a new CSV for loading into an existing script I have with appropriate data types.

#### What do you mean by dbeaver sql-autocomplete compatible?

dbeaver's autocomplete is very good if you have created a table with columns like:

`create table deployment.example`

`(`

`   column_one varchar,`

`   column_two varchar`

`)`

and doesn't guess well if create a table like this:

`create table deployment.bad_example`

`(`

`   "column one" varchar,`

`   "column two" varchar`

`)`

#### What does this currently do?
 - Identify the first line of a CSV with unix linebreaks (windows forthcoming because that's what I work on)
 - Prints out a fixed first line of the CSV in the terminal to replace the first line in the input CSV
 - Prints out columns in the example format above to help with creating a sql table to import the data into

#### Planned Features
 - Detect windows or linux line endings and behave appropriately
 - Produce a file ending with `_sqlized.csv` with the corrected first line
 - Produce a basic sql file giving the entire skeleton of the varchar table to be created
