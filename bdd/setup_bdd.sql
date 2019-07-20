CREATE TABLE tag (name text, r_color integer, g_color integer, b_color integer, PRIMARY KEY(name));
CREATE TABLE document (name text, path text, date date, PRIMARY KEY(path));
CREATE TABLE tag_document_relation (id_tag REFERENCES tag, id_doc REFERENCES document, FOREIGN KEY(ROWID) REFERENCES tag, FOREIGN KEY(ROWID) REFERENCES document, PRIMARY KEY(id_tag,id_doc));

INSERT INTO tag (name, r_color, g_color, b_color) 
VALUES 	('banque',1,1,1),
		('facture',2,2,2),
		('contrat',3,3,3),
		('achat',4,4,4);	
		
INSERT INTO document (name, path, date) 
VALUES 	('test_document', 'c:/', 2019-06-26),
		('test_document_2', 'c:/2', 2019-06-26),
		('test_document_3', 'c:/3', 2019-06-26);

INSERT INTO tag_document_relation (id_tag,id_doc)
VALUES 	(4,1),
		(2,1),
		(1,2),
		(2,2),
		(3,3);
		
DELETE FROM tag;
DELETE FROM document;
DELETE FROM tag_document_relation;
