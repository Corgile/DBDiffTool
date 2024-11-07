---- 所有表格
WITH a AS (
    SELECT
        table_schema, table_name, column_name as cname, is_nullable,
        concat_ws('_', udt_name, character_maximum_length,
                  numeric_precision, numeric_precision_radix, numeric_scale) AS table_types
    FROM information_schema.columns
) SELECT
      table_schema, table_name,
      string_agg(cname,       ',' ORDER BY LENGTH(cname),cname) AS fields,
      string_agg(table_types, ',' ORDER BY LENGTH(cname),cname) AS types,
      string_agg(is_nullable, ',' ORDER BY LENGTH(cname),cname) AS nulls
FROM a WHERE table_schema NOT IN ('pg_catalog', 'information_schema')
GROUP BY table_schema, table_name
ORDER BY LENGTH(table_schema),table_schema,LENGTH(table_name),table_name;

---- 所有表格(外加一行哨兵, 为什么: 与 db::impl::PostgreSQL::SchemaList 实现有关)
WITH a AS (
    SELECT
        table_schema, table_name, column_name as cname, is_nullable,
        concat_ws('_', udt_name, character_maximum_length,
                  numeric_precision, numeric_precision_radix, numeric_scale) AS table_types
    FROM information_schema.columns
), b AS (
    SELECT table_schema, table_name,
           string_agg(cname,       ',' ORDER BY LENGTH(cname),cname) AS fields,
           string_agg(table_types, ',' ORDER BY LENGTH(cname),cname) AS types,
           string_agg(is_nullable, ',' ORDER BY LENGTH(cname),cname) AS nulls
    FROM a WHERE table_schema NOT IN ('pg_catalog', 'information_schema')
    GROUP BY table_schema, table_name
    ORDER BY LENGTH(table_schema), table_schema, LENGTH(table_name), table_name
) SELECT * FROM b UNION ALL (SELECT 'drop', 'drop', 'drop', 'drop', 'drop');

---- 所有视图(外加一行哨兵, 为什么: 与 db::impl::PostgreSQL::SchemaList 实现有关)
WITH a AS (
    SELECT c.table_schema, c.table_name, c.column_name as cname, c.is_nullable,
           concat_ws('_', c.udt_name, c.character_maximum_length, c.numeric_precision,
                     c.numeric_precision_radix, c.numeric_scale) AS table_types
    FROM information_schema.columns as c INNER JOIN information_schema.tables as t
         ON c.table_schema = t.table_schema AND c.table_name = t.table_name
    WHERE t.table_type = 'VIEW' AND c.table_schema NOT IN ('pg_catalog', 'information_schema')
), b AS (
    SELECT
        table_schema, table_name,
        string_agg(cname,       ',' ORDER BY LENGTH(cname),cname) AS fields,
        string_agg(table_types, ',' ORDER BY LENGTH(cname),cname) AS types,
        string_agg(is_nullable, ',' ORDER BY LENGTH(cname),cname) AS nulls
    FROM a GROUP BY table_schema, table_name
    ORDER BY LENGTH(table_schema), table_schema, LENGTH(table_name), table_name
) SELECT * FROM b UNION ALL (SELECT 'drop', 'drop', 'drop', 'drop', 'drop');

---- 索引
WITH index_info AS (
    SELECT t.relname AS table_name,
           i.relname AS index_name,
           STRING_AGG(a.attname, ',' ORDER BY a.attnum) AS indexed_fields,
           md5(pg_get_indexdef(i.oid)) AS index_md5
    FROM pg_index AS idx
           JOIN pg_class AS i ON i.oid = idx.indexrelid
           JOIN pg_class AS t ON t.oid = idx.indrelid
           JOIN pg_namespace AS n ON n.oid = t.relnamespace
           JOIN pg_attribute AS a ON a.attrelid = t.oid
           JOIN UNNEST(idx.indkey) WITH ORDINALITY AS k(attnum, ord) ON a.attnum = k.attnum
    WHERE n.nspname NOT IN ('pg_toast', 'pg_catalog', 'information_schema')
    GROUP BY n.nspname, t.relname, i.relname, i.oid
) SELECT table_name, index_name, indexed_fields, index_md5
FROM index_info ORDER BY LENGTH(table_name), table_name, LENGTH(index_name),index_name;

---- 序列
SELECT
    sch.nspname	   AS schema_name,
    cls.relname 	   AS seq_name,
    seq.seqmin 	   AS seq_min,
    seq.seqincrement AS seq_incr,
    seq.seqmax 	   AS seq_max,
    seq.seqcycle 	   AS cycle
FROM pg_class      AS cls
         JOIN pg_namespace  AS sch ON sch.oid = cls.relnamespace
         JOIN pg_sequence   AS seq ON seq.seqrelid = cls.oid
WHERE cls.relkind = 'S'
  AND sch.nspname NOT IN ('pg_toast', 'pg_catalog', 'information_schema')
ORDER BY LENGTH(sch.nspname),sch.nspname,LENGTH(cls.relname),seq_name;

---- 存储过程
SELECT
    n.nspname AS schema_name,
    p.proname AS procd_name,
    MD5(pg_get_functiondef(p.oid)) AS procd_md5
FROM pg_proc AS p
         JOIN pg_namespace AS n ON n.oid = p.pronamespace
WHERE p.prokind = 'f' AND n.nspname NOT IN ('pg_toast', 'pg_catalog', 'information_schema')
ORDER BY LENGTH(n.nspname),schema_name,LENGTH(p.proname),procd_name;