SELECT name AS seq_name,
       1 AS seq_min,
       1 AS seq_incr,
       CAST(0x7FFFFFFFFFFFFFFF AS INTEGER) AS seq_max,
       "false" AS cycle
FROM sqlite_sequence ORDER BY LENGTH(name),name;

SELECT
    tbl_name AS table_name, name AS index_name,
    REPLACE(SUBSTR(sql, INSTR(sql, '(') + 1, INSTR(sql, ')')
        - INSTR(sql, '(') - 1), ' ', ', ') AS indexed_fields,
    LOWER(sql) AS index_detail
FROM sqlite_master WHERE type = 'index' AND name NOT LIKE 'sqlite_%'
ORDER BY LENGTH(tbl_name), tbl_name, LENGTH(name),name;


SELECT tbl_name AS table_name, name AS trigger_name, sql AS trigger_detail
FROM sqlite_master WHERE type = 'trigger'
ORDER BY LENGTH(tbl_name), tbl_name, LENGTH(name),name;

SELECT
    m.name AS table_name,
    GROUP_CONCAT(i.name, ',' ORDER BY LENGTH(i.name),i.name) AS table_fields,
    GROUP_CONCAT(i.type, ',' ORDER BY LENGTH(i.name),i.name) AS field_types,
    GROUP_CONCAT(CASE WHEN i."notnull" = 0 THEN 'YES'
                      ELSE 'NO' END, ',' ORDER BY LENGTH(i.name),i.name) AS field_nulls
FROM sqlite_master AS m
         JOIN pragma_table_info(m.name) AS i
WHERE m.type = 'table' AND m.name NOT LIKE 'sqlite_%'
GROUP BY m.name ORDER BY LENGTH(m.name),m.name;


