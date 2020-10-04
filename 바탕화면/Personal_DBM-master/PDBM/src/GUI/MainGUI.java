package GUI;

import java.awt.BorderLayout;
import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.EmptyBorder;
import javax.swing.JTable;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JTextField;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;

public class MainGUI extends JFrame {

	private JPanel contentPane;
	private JTable table;
	private JTextField txtValue;
	private JTextField txtFilename;
	private JTextField txtFilename_1; 
	private JTable table_1;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					  String header[]={"�̸�", "����", "����", "����"};
				        String contents[][]={
				                {"������", "50", "60", "70,"},
				                {"�迵ȣ", "70", "80", "75"},
				                {"������", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                {"������", "85", "60", "85"},
				                {"�����", "80", "65", "95"},
				                {"�迵��", "80", "65", "95"},
				                {"������", "80", "65", "95"},
				                {"�̽±�", "80", "65", "95"},
				                
				        };
					MainGUI frame = new MainGUI(header,contents);
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public MainGUI(String[] field, String[][] record ) {
		setTitle("Data Table");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(300, 300, 450, 300);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);
		
		JPanel panel_1 = new JPanel();
		panel_1.setBounds(12, 10, 136, 241);
		contentPane.add(panel_1);
		panel_1.setLayout(null);
		
		txtValue = new JTextField();
		txtValue.setText("value");
		txtValue.setBounds(0, 0, 136, 21);
		panel_1.add(txtValue);
		txtValue.setColumns(10);
		
		JButton btnNewButton_2 = new JButton("Add value");
		btnNewButton_2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
			}
		});
		btnNewButton_2.setBounds(39, 31, 97, 23);
		panel_1.add(btnNewButton_2);
		
		txtFilename = new JTextField();
		txtFilename.setText("file_name");
		txtFilename.setBounds(0, 81, 136, 21);
		panel_1.add(txtFilename);
		txtFilename.setColumns(10);
		
		JButton btnNewButton_3 = new JButton("Save");
		btnNewButton_3.setBounds(39, 112, 97, 23);
		panel_1.add(btnNewButton_3);
		
		txtFilename_1 = new JTextField();
		txtFilename_1.setText("file_name");
		txtFilename_1.setBounds(0, 160, 136, 21);
		panel_1.add(txtFilename_1);
		txtFilename_1.setColumns(10);
		
		JButton btnNewButton_4 = new JButton("Load");
		btnNewButton_4.setBounds(39, 191, 97, 23);
		panel_1.add(btnNewButton_4);
		
		JPanel panel = new JPanel();
		panel.setBounds(160, 202, 262, 49);
		contentPane.add(panel);
		panel.setLayout(null);
		
		JButton btnNewButton = new JButton("Set field");
		btnNewButton.setBounds(12, 10, 97, 23);
		panel.add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton("Del value");
		btnNewButton_1.setBounds(153, 10, 97, 23);
		panel.add(btnNewButton_1);
		
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(160, 10, 262, 182);
		contentPane.add(scrollPane);
		
		table_1 = new JTable(record,field);
		table_1.setColumnSelectionAllowed(true);
		table_1.setCellSelectionEnabled(true);
		scrollPane.setViewportView(table_1);
	}
}
