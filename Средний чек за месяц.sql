SELECT t.year, t.month, AVG(s.amount) AS average_ticket 
FROM sales_fact s 
JOIN time_dim t ON s.sale_date = t.date 
GROUP BY t.year, t.month 
ORDER BY t.year, t.month;
