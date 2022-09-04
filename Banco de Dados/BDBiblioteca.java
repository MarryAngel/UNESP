import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

public class BDBiblioteca{
    
    BDBiblioteca(){
        new Biblioteca(); 
    }
    
    public static void main (String[] args){
        new BDBiblioteca();
    }
    
    class Biblioteca extends JFrame implements ActionListener{
        Connection con;
        Statement stmt;
        JDesktopPane desktop;
        
        // itens do menu
        JMenuItem menuEmprestar;
        JMenuItem menuMostrarEmprestimos;
        JMenuItem menuRemoverEmprestimos;
        JMenuItem menuConsultarLivros;
        JMenuItem menuCadastrarLeitor;
        JMenuItem menuAtualizarLeitor;
        JMenuItem menuRemoverLeitor;
        JMenuItem menuConsultarLeitor;
        JMenuItem menuTermina;
        
        //janelas
        JanelaConsultaLivros jCL;
        JanelaCadastrarLeitor jCLeitor;
        JanelaAtualizarLeitor jALeitor;
        JanelaRemoverLeitor jRLeitor;
        JanelaConsultarLeitor jConsultarLeitor;
        JanelaMostrarEmprestimos jME;
        JanelaRemoverEmprestimos jRE;
        
        public Biblioteca(){
            super("Biblioteca");
    
            setBounds(50,50,700,500);
            setExtendedState(JFrame.MAXIMIZED_BOTH);
			setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); 
            
            desktop = new JDesktopPane();
            add(desktop);
            
            setJMenuBar(criaMenu());
            
            iniciaBD();
            
            // instanciar janela de empréstimos
            jCL = new JanelaConsultaLivros(desktop, con);
            desktop.add(jCL);
            jCL.setVisible(false);
            
            jME = new JanelaMostrarEmprestimos(desktop, con);
            desktop.add(jME);
            jME.setVisible(false);
            
            jRE = new JanelaRemoverEmprestimos(desktop,con);
            desktop.add(jRE);
            jRE.setVisible(false);
                
            jCLeitor = new JanelaCadastrarLeitor(desktop, con);
            desktop.add(jCLeitor);
            jCLeitor.setVisible(false);
            
            jALeitor = new JanelaAtualizarLeitor(desktop, con);
            desktop.add(jALeitor);
            jALeitor.setVisible(false);
            
            jRLeitor = new JanelaRemoverLeitor(desktop, con);
            desktop.add(jRLeitor);
            jRLeitor.setVisible(false);
            
            jConsultarLeitor = new JanelaConsultarLeitor(desktop, con);
            desktop.add(jConsultarLeitor);
            jConsultarLeitor.setVisible(false);
            
            setVisible(true);    
        }
        
        JMenuBar criaMenu(){
            JMenuBar menuBar = new JMenuBar();
            JMenu menuBD = new JMenu("Operacoes no Banco de Dados");
            menuBar.add(menuBD);
            
            // instanciar itens do menu
            menuEmprestar = new JMenuItem("Realizar um emprestimo", KeyEvent.VK_I);
            menuBD.add(menuEmprestar);
            
            menuMostrarEmprestimos = new JMenuItem("Mostrar emprestimos", KeyEvent.VK_C);
            menuBD.add(menuMostrarEmprestimos);
            
            menuRemoverEmprestimos = new JMenuItem("Remover emprestimos", KeyEvent.VK_I);
            menuBD.add(menuRemoverEmprestimos);
            
            menuConsultarLivros = new JMenuItem("Consultar livros", KeyEvent.VK_C);
            menuBD.add(menuConsultarLivros);
            
            menuCadastrarLeitor = new JMenuItem("Cadastrar leitor", KeyEvent.VK_I);
            menuBD.add(menuCadastrarLeitor);
            
            menuAtualizarLeitor = new JMenuItem("Atualizar leitor", KeyEvent.VK_I);
            menuBD.add(menuAtualizarLeitor);
            
            menuRemoverLeitor = new JMenuItem("Remover leitor", KeyEvent.VK_I);
            menuBD.add(menuRemoverLeitor);
            
            menuConsultarLeitor = new JMenuItem("Consultar leitor", KeyEvent.VK_C);
            menuBD.add(menuConsultarLeitor);
            
            menuTermina = new JMenuItem("Termina");
			menuBar.add(menuTermina);
            
            menuEmprestar.addActionListener(this);
            menuMostrarEmprestimos.addActionListener(this);
            menuRemoverEmprestimos.addActionListener(this);
            menuConsultarLivros.addActionListener(this);
            menuCadastrarLeitor.addActionListener(this);
            menuAtualizarLeitor.addActionListener(this);
            menuRemoverLeitor.addActionListener(this);  
            menuConsultarLeitor.addActionListener(this); 
            menuTermina.addActionListener(this);
            
            return menuBar;
        }
        
        public void actionPerformed(ActionEvent e){
            if (e.getSource() == menuEmprestar){
                new JanelaEmprestar(desktop, con);
            } else if (e.getSource() == menuMostrarEmprestimos){
                jME.setVisible(true); 
            } else if (e.getSource() == menuRemoverEmprestimos){
                jRE.setVisible(true);     
            } else if (e.getSource() == menuConsultarLivros){
                jCL.setVisible(true); 
            } else if (e.getSource() == menuCadastrarLeitor){
                jCLeitor.setVisible(true); 
            } else if (e.getSource() == menuAtualizarLeitor){
                jALeitor.setVisible(true); 
            } else if (e.getSource() == menuRemoverLeitor){
                jRLeitor.setVisible(true); 
            } else if (e.getSource() == menuConsultarLeitor){
                jConsultarLeitor.setVisible(true); 
            } else if (e.getSource() == menuTermina){
                System.exit(0);
            }
        }
        
        //iniciação do banco
        void iniciaBD(){
			try{
				Class.forName("org.hsql.jdbcDriver");
                con = DriverManager.getConnection("jdbc:HypersonicSQL:hsql://localhost:8081", "sa", "");
                stmt = con.createStatement();
			} catch (ClassNotFoundException ex) {
				JOptionPane.showMessageDialog(null, "O driver do banco de dados não foi encontrado.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
				System.exit(1);
			} catch (SQLException ex) {
				JOptionPane.showMessageDialog(null, "Erro na iniciação do acesso ao banco de dados\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
				System.exit(1);
			}
		}
        
        public void finalize(){
			try{
				stmt.close();
				con.close();
			} catch (SQLException e){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
		}
    }
    
    class JanelaEmprestar extends JInternalFrame{
        PreparedStatement pStmt;
        JDesktopPane desktop;
        JButton bt1;
        JTextField tf1, tf2, tf3, tf4, tf5;
        
        public JanelaEmprestar(JDesktopPane d, Connection con){
            super("Registro de emprestimo", false, true, false, true);
            desktop = d;
            
            try{
                //comando sql
                pStmt = con.prepareStatement("INSERT INTO EMPRESTAR VALUES(?, ?, ?, ?, ?)");
                
                setLayout(new FlowLayout());
                
                add(new JLabel("CPF: "));
				add(tf1 = new JTextField(15));
                
                add(new JLabel("ISBN: "));
				add(tf2 = new JTextField(15));
                
                add(new JLabel("Codigo: "));
				add(tf3 = new JTextField(15));
                
                add(new JLabel("Data emprestimo: "));
				add(tf4 = new JTextField(10));
                
                add(new JLabel("Data devolucao: "));
				add(tf5 = new JTextField(10));
                
                add(bt1 = new JButton("Inserir"));
                pack();
                setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
				setVisible(true);
				desktop.add(this);
                
                bt1.addActionListener(new ActionListener(){
						public void actionPerformed(ActionEvent e){
							try{
								pStmt.setString(1, tf1.getText());
								pStmt.setString(2, tf2.getText());
								pStmt.setString(3, tf3.getText());
								pStmt.setString(4, tf4.getText());
                                pStmt.setString(5, tf5.getText());
                                
								tf1.setText("");
								tf2.setText("");
								tf3.setText("");
								tf4.setText("");
                                tf5.setText("");
                                
								pStmt.executeUpdate();
							} catch(Exception ex){
								JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
							}
						}
					});
            } catch (Exception ex) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
            }	
            
        }
    }
    
    class JanelaConsultaLivros extends JInternalFrame implements ActionListener{
        PreparedStatement pStmt;
		JDesktopPane desktop;
		JButton bt1;
		JTextField tf1;
		JTextArea tal;
    
        
        public JanelaConsultaLivros(JDesktopPane d, Connection con){
            super("Consulta de livros", false, true, false, true);
            desktop = d;
            
            try{
				pStmt = con.prepareStatement("SELECT * FROM LIVRO_FISICO WHERE ISBN LIKE ?");
					
				JPanel l1 = new JPanel();
				l1.add(new JLabel("ISBN: "));
				l1.add(tf1 = new JTextField(15));
				l1.add(bt1 = new JButton("Pesquisar"));
				add(l1, BorderLayout.NORTH);
				l1 = new JPanel();
				JScrollPane scrollPane = new JScrollPane(tal = new JTextArea(5, 65));
				l1.add(scrollPane);
				add(l1, BorderLayout.CENTER);
				
				bt1.addActionListener(this);
				pack();
				setDefaultCloseOperation(HIDE_ON_CLOSE);

			} catch (Exception ex) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
			}
        }
        
        public void actionPerformed(ActionEvent e){
			try{
				tal.setText("");
				pStmt.setString(1, tf1.getText());
				ResultSet rs = pStmt.executeQuery();
				
				while(rs.next()){
					String ISBN = rs.getString(1);
					String titulo = rs.getString(2);
                    String categoria = rs.getString(3);
                    String edicao = rs.getString(4);
                    String ano_lancamento = rs.getString(5);
                    String idioma = rs.getString(6);
                    String editora = rs.getString(7);
                    String disponibilidade = rs.getString(8);
                    String qtd = rs.getString(9);
					//tal.append(s + "   " + n + "\n");
                tal.append("ISBN: "+ ISBN + "    " + "Titulo: " + titulo + "    " + "Disponibilidade: " + disponibilidade + "    " + "Quantidade: " + qtd +"\n");
				}
				
				tf1.selectAll();
			} catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
		}
        
        public void finalize(){
			try {
				pStmt.close();
			} catch (SQLException e) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);
			}
		}	
        
    }
    
    class JanelaCadastrarLeitor extends JInternalFrame implements ActionListener{
        PreparedStatement pStmt;
		JDesktopPane desktop;
		JButton bt1;
		JTextField tf1, tf2, tf3, tf4;
        
        public JanelaCadastrarLeitor(JDesktopPane d, Connection con){
            super("Registro de leitor", false, true, false, true);
            desktop = d;
            
            try{
               pStmt = con.prepareStatement("INSERT INTO LEITOR VALUES(?, ?, ?, ?, 0, 0, 'Liberado')"); 
               
               JPanel l1 = new JPanel();
               l1.add(new JLabel("CPF: "));
               l1.add(tf1 = new JTextField(15));
               l1.add(new JLabel("Nome: "));
               l1.add(tf2 = new JTextField(20));
               l1.add(new JLabel("CEP: "));
               l1.add(tf3 = new JTextField(15));
               l1.add(new JLabel("Numero: "));
               l1.add(tf4 = new JTextField(5));
               l1.add(bt1 = new JButton("Inserir"));
               add(l1, BorderLayout.CENTER);
                
               bt1.addActionListener(this);
               pack();
               setDefaultCloseOperation(HIDE_ON_CLOSE);
                
            } catch (Exception ex) {
					JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
            }
            
        }
        
        public void actionPerformed(ActionEvent e){
			try{
				pStmt.setString(1, tf1.getText());
				pStmt.setString(2, tf2.getText());
                pStmt.setString(3, tf3.getText());
                pStmt.setString(4, tf4.getText());
                tf1.setText("");
				tf2.setText("");
				tf3.setText("");
                tf4.setText("");
                pStmt.executeUpdate();
			} catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
            
            JOptionPane.showMessageDialog(desktop, "Cliente inserido com sucesso");
		}
        
        public void finalize(){
			try {
				pStmt.close();
			} catch (SQLException e) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);
			}
		}	
    }
    
    class JanelaAtualizarLeitor extends JInternalFrame implements ActionListener{
        PreparedStatement pStmt, pStmt2;
		JDesktopPane desktop;
		JButton bt1, bt2;
		JTextField tf1, tf2, tf3, tf4;
		JTextArea tal;
        
        public JanelaAtualizarLeitor(JDesktopPane d, Connection con){
            super("Alteracao de registro", false, true, false, true);
            desktop = d;
            
            try{
               pStmt = con.prepareStatement("SELECT * FROM LEITOR WHERE CPF LIKE ?"); 
               pStmt2 = con.prepareStatement("UPDATE LEITOR SET NOME = ?, CEP = ?, NUMERO_END = ? WHERE CPF LIKE ?");    
               
               JPanel l1 = new JPanel();
               l1.add(new JLabel("CPF: "));
               l1.add(tf1 = new JTextField(15));
               l1.add(bt1 = new JButton("Pesquisar"));
               add(l1, BorderLayout.PAGE_START);   
               
               l1 = new JPanel();
               JScrollPane scrollPane = new JScrollPane(tal = new JTextArea(2, 50));
               l1.add(scrollPane);
               add(l1, BorderLayout.CENTER);        
				
               l1 = new JPanel();   
               l1.add(new JLabel("Nome: "));
               l1.add(tf2 = new JTextField(20));
               l1.add(new JLabel("CEP: "));
               l1.add(tf3 = new JTextField(15));
               l1.add(new JLabel("Numero: "));
               l1.add(tf4 = new JTextField(5));
               l1.add(bt2 = new JButton("Atualizar"));
               add(l1, BorderLayout.PAGE_END);
                
               bt1.addActionListener(new ActionListener(){
						public void actionPerformed(ActionEvent e){
							try{
								tal.setText("");
                                pStmt.setString(1, tf1.getText());
                                ResultSet rs = pStmt.executeQuery();
                                
                                
								while(rs.next()){
					               String cpf = rs.getString(1);
                                   String nome = rs.getString(2);
                                   String CEP = rs.getString(3);
                                   String numero = rs.getString(4);    
                                   tal.append("CPF: " + cpf + "     " + "Nome: " + nome + "     " + "CEP: " + CEP + "     " + "Numero: " + numero +"\n");
				                }
				
				                tf1.selectAll();
							} catch(Exception ex){
								JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
							}
						}
					});
                
               bt2.addActionListener(new ActionListener(){
						public void actionPerformed(ActionEvent e){
							try{
				                pStmt2.setString(1,tf2.getText());
                                pStmt2.setString(2,tf3.getText());
                                pStmt2.setString(3,tf4.getText());
                                pStmt2.setString(4,tf1.getText());
								tf2.setText("");
								tf3.setText("");
								tf4.setText("");
								pStmt2.executeUpdate();
							} catch(Exception ex){
								JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
							}
						}
					}); 
               pack();
               setDefaultCloseOperation(HIDE_ON_CLOSE);
                
            } catch (Exception ex) {
					JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
            }
            
        }
        
        public void actionPerformed(ActionEvent e){
			try{
				tal.setText("");
				pStmt.setInt(1, Integer.parseInt(tf1.getText()));
				pStmt.setString(2, tf2.getText());
                pStmt.setInt(3, Integer.parseInt(tf3.getText()));
                pStmt.setInt(4, Integer.parseInt(tf4.getText()));
                tf1.setText("");
				tf2.setText("");
				tf3.setText("");
                tf4.setText("");
                pStmt.executeUpdate();
			} catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
		}
        
        public void finalize(){
			try {
				pStmt.close();
			} catch (SQLException e) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);
			}
		}	
    }
    
    class JanelaRemoverLeitor extends JInternalFrame implements ActionListener{
        PreparedStatement pStmt;
		JDesktopPane desktop;
		JButton bt1;
		JTextField tf1;
		JTextArea tal;
        
        public JanelaRemoverLeitor(JDesktopPane d, Connection con){
            super("Remover leitor", false, true, false, true);
            desktop = d;
         
            try{
				pStmt = con.prepareStatement("DELETE FROM LEITOR WHERE CPF LIKE ?");
					
				JPanel l1 = new JPanel();
				l1.add(new JLabel("CPF do leitor: "));
				l1.add(tf1 = new JTextField(30));
				l1.add(bt1 = new JButton("Deletar"));
				add(l1, BorderLayout.CENTER);
				
				bt1.addActionListener(this);
				pack();
				setDefaultCloseOperation(HIDE_ON_CLOSE);

			} catch (Exception ex) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
			}  
        }
        
        public void actionPerformed(ActionEvent e){
			try{
				pStmt.setString(1, tf1.getText());
                tf1.setText("");

                pStmt.executeUpdate();
			} catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
		}
        
        public void finalize(){
			try {
				pStmt.close();
			} catch (SQLException e) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);
			}
		}
    }
    
    class JanelaConsultarLeitor extends JInternalFrame implements ActionListener{
        PreparedStatement pStmt;
		JDesktopPane desktop;
		JButton bt1;
		JTextField tf1;
		JTextArea tal;
        
        public JanelaConsultarLeitor(JDesktopPane d, Connection con){
            super("Consulta de leitor registrado", false, true, false, true);
            desktop = d;
            
            try{
                pStmt = con.prepareStatement("SELECT * FROM LEITOR WHERE CPF LIKE ?");
                
                JPanel l1 = new JPanel();
				l1.add(new JLabel("CPF: "));
				l1.add(tf1 = new JTextField(30));
				l1.add(bt1 = new JButton("Pesquisar"));
				add(l1, BorderLayout.NORTH);
				l1 = new JPanel();
				JScrollPane scrollPane = new JScrollPane(tal = new JTextArea(6, 85));
				l1.add(scrollPane);
				add(l1, BorderLayout.CENTER);
                
                bt1.addActionListener(this);
                pack();
                setDefaultCloseOperation(HIDE_ON_CLOSE);
            } catch (Exception ex) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
			}
        }
        
        public void actionPerformed(ActionEvent e){
			try{
				tal.setText("");
				pStmt.setString(1, tf1.getText());
				ResultSet rs = pStmt.executeQuery();
				
				while(rs.next()){
					String cpf = rs.getString(1);
                    String nome = rs.getString(2);
                    String cep = rs.getString(3);
                    String numero_end = rs.getString(4);
                    String qtd_emprestado = rs.getString(5);
                    String qtd_faltas = rs.getString(6);
                    String situacao = rs.getString(7);

                    tal.append("CPF: " + cpf + "     " + "Nome: " + nome + "     " + "CEP: " + cep + "     " + "Numero: " + numero_end + "     " + "Quantidade emprestada: " + qtd_emprestado + "     " + "Quantidade faltas: " + qtd_faltas + "     " + "Situacao: " + situacao +"\n");
				}
				
				tf1.selectAll();
			} catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
		}
        
        public void finalize(){
			try {
				pStmt.close();
			} catch (SQLException e) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);
			}
		}
    }
    
    class JanelaMostrarEmprestimos extends JInternalFrame implements ActionListener{
        PreparedStatement pStmt;
		JDesktopPane desktop;
		JButton bt1;
		JTextField tf1;
		JTextArea tal;
        
        public JanelaMostrarEmprestimos(JDesktopPane d, Connection con){
            super("Relatorio de emprestimos", false, true, false, true);
            desktop = d;
            
            try{
                pStmt = con.prepareStatement("SELECT * FROM EMPRESTAR");
                JPanel l1 = new JPanel();
                l1.add(bt1 = new JButton("Atualizar"));
                add(l1,BorderLayout.PAGE_START);
                l1 = new JPanel();
				JScrollPane scrollPane = new JScrollPane(tal = new JTextArea(20, 80));
				l1.add(scrollPane);
				add(l1, BorderLayout.CENTER);
                
                bt1.addActionListener(this);
                pack();
				setDefaultCloseOperation(HIDE_ON_CLOSE);

            }  catch (Exception ex) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
			}   
        
            try{
                tal.setText("");
                ResultSet rs = pStmt.executeQuery();
                
                while(rs.next()){
                    String cpf = rs.getString(1);
                    String isbn = rs.getString(2);
                    String codigo = rs.getString(3);
                    String d_e = rs.getString(4);
                    String d_d = rs.getString(5);
                    
                    tal.append("CPF: " + cpf + "    " + "ISBN: " + isbn + "    " + "Codigo: " + codigo + "    " + "Data emprestimo: " + d_e + "    " + "Data devolucao: " + d_d +"\n");
                }
            } catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			} 
        }
        
        public void actionPerformed(ActionEvent e){
            try{
                tal.setText("");
                ResultSet rs = pStmt.executeQuery();
                
                while(rs.next()){
                    String cpf = rs.getString(1);
                    String isbn = rs.getString(2);
                    String codigo = rs.getString(3);
                    String d_e = rs.getString(4);
                    String d_d = rs.getString(5);
                    
                    tal.append("CPF: " + cpf + "    " + "ISBN: " + isbn + "    " + "Codigo: " + codigo + "    " + "Data emprestimo: " + d_e + "    " + "Data devolucao: " + d_d +"\n");
                }
            } catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
		}
        
        public void finalize(){
			try {
				pStmt.close();
			} catch (SQLException e) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);
			}
		}
    }
    
    class JanelaRemoverEmprestimos extends JInternalFrame implements ActionListener{
        PreparedStatement pStmt;
		JDesktopPane desktop;
		JButton bt1;
		JTextField tf1, tf2, tf3;
		JTextArea tal;
        
        public JanelaRemoverEmprestimos(JDesktopPane d, Connection con){
            super("Remover emprestimos", false, true, false, true);
            desktop = d;

            try{
                pStmt = con.prepareStatement("DELETE FROM EMPRESTAR WHERE CPF LIKE ? AND ISBN LIKE ? AND DATA_EMPRESTIMO LIKE ?");
                
                JPanel l1 = new JPanel();
                l1.add(new JLabel("CPF: "));
                l1.add(tf1 = new JTextField(15));
                l1.add(new JLabel("ISBN: "));
                l1.add(tf2 = new JTextField(15));
                l1.add(new JLabel("Data de emprestimo: "));
                l1.add(tf3 = new JTextField(15));
                l1.add(bt1 = new JButton("Deletar"));
                add(l1, BorderLayout.CENTER);

                bt1.addActionListener(this);
                pack();
                setDefaultCloseOperation(HIDE_ON_CLOSE);

            } catch (Exception ex) {
                    JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);
            }     
        }
        
         public void actionPerformed(ActionEvent e){
			try{
				pStmt.setString(1, tf1.getText());
                pStmt.setString(2, tf2.getText());
                pStmt.setString(3, tf3.getText());
                tf1.setText("");
                tf2.setText("");
                tf3.setText("");
                
                pStmt.executeUpdate();
			} catch (Exception ex){
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+ex, "Erro", JOptionPane.ERROR_MESSAGE);	
			}
		}
        
        public void finalize(){
			try {
				pStmt.close();
			} catch (SQLException e) {
				JOptionPane.showMessageDialog(desktop, "Problema interno.\n"+e, "Erro", JOptionPane.ERROR_MESSAGE);
			}
		}
    }
}