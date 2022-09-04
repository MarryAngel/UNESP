import java.sql.*;

class criaBiblioteca{
  public static void main(String[] args) {
    try {
		Class.forName("org.hsql.jdbcDriver");
		Connection con = DriverManager.getConnection("jdbc:HypersonicSQL:hsql://localhost:8081", "sa", "");
		Statement stmt = con.createStatement();
		stmt.executeUpdate("create table endereco(cep varchar(8), rua varchar(50), bairro varchar(30),primary key(cep))");
		stmt.executeUpdate("create table leitor(cpf varchar(11),nome varchar(50),cep varchar(8),numero_end varchar(5),qtd_emprestado varchar(1),qtd_faltas varchar(1),situacao varchar(10),primary key(cpf),foreign key (cep) references endereco(cep))");
		stmt.executeUpdate("create table telefone_leitor(cpf varchar(11), telefone varchar(11) not null,primary key (cpf, telefone),foreign key(cpf) references leitor(cpf))");
		stmt.executeUpdate("create table ebook(isbn varchar(13),titulo varchar(100) not null,categoria varchar(30),edicao varchar(10),ano_lancamento varchar(4),idioma varchar(20),tamanho varchar(5) not null,versao varchar(3), primary key (isbn))");
		stmt.executeUpdate("create table autor_ebook(isbn varchar(13),nome varchar(100), primary key (isbn, nome), foreign key (isbn) references ebook(isbn))");
		stmt.executeUpdate("create table acessar(cpf varchar(11), isbn varchar(13),primary key(cpf, isbn),foreign key (cpf) references leitor(cpf),foreign key (isbn) references ebook(isbn))");
		stmt.executeUpdate("create table livro_fisico(isbn varchar(13),titulo varchar(100) not null,categoria varchar(30),edicao varchar(10),ano_lancamento varchar(4),idioma varchar(20),editora varchar(50),disponibilidade varchar(15),qtd varchar(2),primary key(isbn))");
		stmt.executeUpdate("create table autor_fisico(isbn varchar(13),nome varchar(100),primary key (isbn, nome),foreign key (isbn) references livro_fisico(isbn))");
		stmt.executeUpdate("create table exemplar(isbn varchar(13),codigo varchar(5),conservacao varchar(15),tipo_capa varchar(15),primary key (isbn, codigo),foreign key (isbn) references livro_fisico(isbn))");
		stmt.executeUpdate("create table emprestar(cpf varchar(11), isbn varchar(13),codigo varchar(5),data_emprestimo varchar(10), data_devolucao varchar(10),primary key(cpf, isbn, codigo, data_emprestimo),foreign key (isbn,codigo) references exemplar(isbn,codigo),foreign key (cpf) references leitor(cpf))");
		stmt.executeUpdate("create table doar(cpf varchar(11), isbn varchar(13),codigo varchar(5),primary key(cpf, isbn, codigo), foreign key (cpf) references leitor(cpf), foreign key (isbn, codigo) references exemplar(isbn, codigo))");
		stmt.executeUpdate("create table bibliotecario(ctps varchar(10), cpf varchar(11), nome varchar(50), telefone varchar(11) not null, cep varchar(8),numero_end varchar(5),salario varchar(8),primary key (ctps),foreign key (cep) references endereco(cep))");
		stmt.executeUpdate("create table fornecedor(cnpj varchar(14),nome varchar(50),primary key(cnpj))");
		stmt.executeUpdate("create table solicitar(ctps varchar(10),isbn varchar(13),numero varchar(10) ,primary key (ctps, isbn, numero),foreign key (ctps) references bibliotecario(ctps),foreign key (isbn) references livro_fisico(isbn))");
		stmt.executeUpdate("create table pedido (numero varchar(10),cnpj varchar(14),data_pedido varchar(10), preco varchar(8), primary key (cnpj, numero),foreign key (cnpj) references fornecedor(cnpj),foreign key (numero) references solicitar(numero))");
		stmt.close();
		con.close();
    } catch (Exception e) {
		System.out.println(e);
    }
  }
}