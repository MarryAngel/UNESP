import java.sql.*;

class insereBiblioteca {
  public static void main(String[] args) {
    try {
      Class.forName("org.hsql.jdbcDriver");
      Connection con = DriverManager.getConnection("jdbc:HypersonicSQL:hsql://localhost:8081", "sa", "");
      Statement stmt = con.createStatement();
	  
	  //INSERÇÂO TABELA ENDERECO
      stmt.executeUpdate("insert into endereco (cep, rua , bairro) values ('11111111', 'Rua das Camelias', 'Bairro das Flores')");
	  stmt.executeUpdate("insert into endereco (cep, rua, bairro) values ('22222222', 'Rua Chocolate', 'Bairro Comida')");
	  stmt.executeUpdate("insert into endereco (cep, rua, bairro) values ('33333333', 'Rua Pudim', 'Bairro Comida')");
	  stmt.executeUpdate("insert into endereco (cep, rua, bairro) values ('44444444', 'Rua Gato', 'Bairro Animal')");
	  
	  //INSERÇÂO TABELA LEITOR
	  stmt.executeUpdate("insert into leitor (cpf, nome, cep, numero_end, qtd_emprestado, qtd_faltas, situacao) values ('12345678912', 'Cassiano', '33333333', '54','0','0','Liberado')");
	  stmt.executeUpdate("insert into leitor (cpf, nome, cep, numero_end, qtd_emprestado, qtd_faltas, situacao) values ('74185296348', 'Maria', '44444444', '27','0','0','Liberado')");
	  stmt.executeUpdate("insert into leitor (cpf, nome, cep, numero_end, qtd_emprestado, qtd_faltas, situacao) values ('75315946285', 'Nicole', '22222222', '55','0','0','Liberado')");
	  stmt.executeUpdate("insert into leitor (cpf, nome, cep, numero_end, qtd_emprestado, qtd_faltas, situacao) values ('45612374168', 'Nicolas', '22222222', '55','0','0','Liberado')");
	  stmt.executeUpdate("insert into leitor (cpf, nome, cep, numero_end, qtd_emprestado, qtd_faltas, situacao) values ('75912345826', 'Gustavo', '11111111', '66','0','0','Liberado')");
	  
	  //INSERÇÂO TABELA TELEFONE_LEITOR
	  stmt.executeUpdate("insert into telefone_leitor (cpf, telefone) values ('12345678912','14997584236')");
	  stmt.executeUpdate("insert into telefone_leitor (cpf, telefone) values ('74185296348','14998499116')");
	  stmt.executeUpdate("insert into telefone_leitor (cpf, telefone) values ('75315946285','14998751475')");
	  stmt.executeUpdate("insert into telefone_leitor (cpf, telefone) values ('45612374168','14998561247')");
	  stmt.executeUpdate("insert into telefone_leitor (cpf, telefone) values ('75912345826','15895461475')");
	  stmt.executeUpdate("insert into telefone_leitor (cpf, telefone) values ('12345678912','17998547216')");
	  
	  //INSERÇÂO TABELA EBOOK
	  stmt.executeUpdate("insert into ebook (isbn, titulo, categoria, edicao, ano_lancamento, idioma, tamanho, versao) values ('1122334455664','1984','Distopia','5','1949','Portugues','175','4')");
	  stmt.executeUpdate("insert into ebook (isbn, titulo, categoria, edicao, ano_lancamento, idioma, tamanho, versao) values ('5544778899447','Percy Jackson e o Ladrao de Raios','Aventura','3','2005','Ingles','200','2')");
	  stmt.executeUpdate("insert into ebook (isbn, titulo, categoria, edicao, ano_lancamento, idioma, tamanho, versao) values ('5557774896654','Maze Runner','Ficcao Cientifica','1','2009','Alemao','210','3')");
	  stmt.executeUpdate("insert into ebook (isbn, titulo, categoria, edicao, ano_lancamento, idioma, tamanho, versao) values ('7778889995551','Jogos Mentais','Ficcao Cientifica','4','2015','Portugues','230','1')");
	  stmt.executeUpdate("insert into ebook (isbn, titulo, categoria, edicao, ano_lancamento, idioma, tamanho, versao) values ('4445557775554','Mundo de Sofia','Romance','15','1991','Portugues','300','5')");
	  
	  //INSERÇÂO TABELA AUTOR_EBOOK
	  stmt.executeUpdate("insert into autor_ebook (isbn, nome) values ('4445557775554','Jostein Gaarder')");
	  stmt.executeUpdate("insert into autor_ebook (isbn, nome) values ('7778889995551','Teri Terry')");
	  stmt.executeUpdate("insert into autor_ebook (isbn, nome) values ('1122334455664','George Orwell')");
	  stmt.executeUpdate("insert into autor_ebook (isbn, nome) values ('5544778899447','Rick Riordan')");
	  stmt.executeUpdate("insert into autor_ebook (isbn, nome) values ('5557774896654','James Dashnef')");
	  
	  //INSERÇÃO TABELA ACESSAR
	  stmt.executeUpdate("insert into acessar (cpf, isbn) values ('12345678912','1122334455664')");
	  stmt.executeUpdate("insert into acessar (cpf, isbn) values ('12345678912','4445557775554')");
	  stmt.executeUpdate("insert into acessar (cpf, isbn) values ('45612374168','5544778899447')");
	  
	  //INSERÇÂO TABELA LIVRO_FISICO
	  stmt.executeUpdate("insert into livro_fisico (isbn, titulo, categoria, edicao, ano_lancamento, idioma, editora, disponibilidade, qtd) values ('1122334455664','1984','Distopia','4','1949','Portugues','Warburg','Disponivel','3')");
	  stmt.executeUpdate("insert into livro_fisico (isbn, titulo, categoria, edicao, ano_lancamento, idioma, editora, disponibilidade, qtd) values ('7773335554441','Mochileiro das Galaxias','Ficcao Cientifica','3','1979','Portugues','Panbooks','Disponivel','5')");
	  stmt.executeUpdate("insert into livro_fisico (isbn, titulo, categoria, edicao, ano_lancamento, idioma, editora, disponibilidade, qtd) values ('7779995554442','O Rei Mago','Fantasia','2','2011','Portugues','Estrela','Disponivel','1')");
	  stmt.executeUpdate("insert into livro_fisico (isbn, titulo, categoria, edicao, ano_lancamento, idioma, editora, disponibilidade, qtd) values ('2223334449997','O grande livro de ciencias do Manual do Mundo','Ciencia','1','2016','Portugues','Sextante','Disponivel','2')");
	  stmt.executeUpdate("insert into livro_fisico (isbn, titulo, categoria, edicao, ano_lancamento, idioma, editora, disponibilidade, qtd) values ('5556660003348','Alice no Pais das Maravilhas','Fantasia','3','1865','Ingles','Arqueiro','Disponivel','7')");
	  stmt.executeUpdate("insert into livro_fisico (isbn, titulo, categoria, edicao, ano_lancamento, idioma, editora, disponibilidade, qtd) values ('7799546215762','Fundamentos da Fisica - Vol 1','Ciencia','10','2016','Portugues','LTC','Disponivel','4')");
	  
	  //INSERÇÃO TABELA AUTOR_FISICO
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('1122334455664','George Orwell')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('7773335554441','Douglas Addams')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('7779995554442','Lev Grossman')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('5556660003348','Lewis Carroll')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('2223334449997','Ibere Thenorio')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('2223334449997','Mari Fulfaro')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('7799546215762',' David Halliday')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('7799546215762','Robert Resnick')");
	  stmt.executeUpdate("insert into autor_fisico (isbn, nome) values ('7799546215762','Jearl Walker')");
	  
	  //INSERÇÂO TABELA EXEMPLAR
	  
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('1122334455664','1','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('1122334455664','2','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('1122334455664','3','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7773335554441','1','ruim','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7773335554441','2','ruim','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7773335554441','3','ruim','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7773335554441','4','ruim','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7773335554441','5','ruim','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7779995554442','1','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('2223334449997','1','bom','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('2223334449997','2','bom','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('5556660003348','1','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('5556660003348','2','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('5556660003348','3','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('5556660003348','4','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('5556660003348','5','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('5556660003348','6','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('5556660003348','7','bom','dura')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7799546215762','1','bom','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7799546215762','2','bom','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7799546215762','3','bom','mole')");
	  stmt.executeUpdate("insert into exemplar (isbn, codigo, conservacao, tipo_capa) values ('7799546215762','4','bom','mole')");
	  
		//INSERÇÂO TABELA EMPRESTAR
		stmt.executeUpdate("insert into emprestar (cpf, isbn, codigo, data_emprestimo, data_devolucao) values ('75315946285','5556660003348','5','14/08/2022','30/08/2022')");
		stmt.executeUpdate("insert into emprestar (cpf, isbn, codigo, data_emprestimo, data_devolucao) values ('12345678912','5556660003348','1','22/08/2022','15/09/2022')");
		stmt.executeUpdate("insert into emprestar (cpf, isbn, codigo, data_emprestimo, data_devolucao) values ('74185296348','1122334455664','3','14/08/2022','30/08/2022')");
		stmt.executeUpdate("insert into emprestar (cpf, isbn, codigo, data_emprestimo, data_devolucao) values ('75315946285','7779995554442','1','7/08/2022','20/08/2022')");
	
		//iINSERÇÃO TABELA DOAR
		stmt.executeUpdate("insert into doar (cpf, isbn, codigo) values ('75912345826', '5556660003348', '3')");
		stmt.executeUpdate("insert into doar (cpf, isbn, codigo) values ('75315946285', '7799546215762', '1')");
		stmt.executeUpdate("insert into doar (cpf, isbn, codigo) values ('12345678912', '7773335554441', '4')");
		stmt.executeUpdate("insert into doar (cpf, isbn, codigo) values ('75315946285', '5556660003348', '2')");
		
		//INSERÇÃO TABELA BIBLIOTECARIO
		stmt.executeUpdate("insert into bibliotecario(ctps, cpf, nome, telefone, cep, numero_end, salario) values ('1459762581','45978614534','Zastin','14856321475','33333333','10','10.00')");
		stmt.executeUpdate("insert into bibliotecario(ctps, cpf, nome, telefone, cep, numero_end, salario) values ('1475498216','55442201468','Arissa','14785423651','33333333','11','21.00')");
		
		//INSERÇÃO TABELA FORNECEDOR
		stmt.executeUpdate("insert into fornecedor(cnpj, nome) values ('15936847521468','Americanas')");
		stmt.executeUpdate("insert into fornecedor(cnpj, nome) values ('75413688547226','Amazon')");
		
		//INSERÇÃO TABELA SOLICITAR
		stmt.executeUpdate("insert into solicitar(ctps, isbn, numero) values ('1459762581','7779995554442','1')");
		stmt.executeUpdate("insert into solicitar(ctps, isbn, numero) values ('1475498216','5556660003348','2')");
		stmt.executeUpdate("insert into solicitar(ctps, isbn, numero) values ('1475498216','7779995554442','3')");
		
		//INSERÇÃO TABELA PEDIDO
		stmt.executeUpdate("insert into pedido(numero, cnpj, data_pedido, preco) values ('1','15936847521468','19/08/2022','50.00')");
		stmt.executeUpdate("insert into pedido(numero, cnpj, data_pedido, preco) values ('2','75413688547226','20/08/2022','37.40')");
		stmt.executeUpdate("insert into pedido(numero, cnpj, data_pedido, preco) values ('3','75413688547226','04/06/2022','89.99')");
	
      stmt.close();
      con.close();
    } catch (Exception e) {
      System.out.println(e);
    }
  }
}