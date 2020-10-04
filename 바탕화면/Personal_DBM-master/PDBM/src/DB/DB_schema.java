/**
 * 
 */
package DB;
import java.util.*;
import java.io.*;
/**
 * @author ������
 *
 */
class test{
	public static void main(String[] args) {
			String[] fieldname = {"a","b","c","d"};
			String[] fieldtype = {"STRING","INTEGER","DOUBLE","CHAR"};
			int fieldnum = 4;
			DBM db = new DBM(fieldnum,fieldname,fieldtype);
			db.addRecord();
			System.out.println(db.record.size());
			db.addValue("first");
			db.addValue("1");
			db.addValue("second");
			db.addValue("thrd");
			db.addValue("aaaaaaaaaa");
			db.addValue("123424");
			db.addValue("aaa");
			db.addValue("aaaa");
			db.addValue("aaaaaa");
			db.setSortfield(0);
			
			for(int i=0;i<4;i++) {
				System.out.println(db.record.get(i).fieldContainer.get(i));	
			}
			
			
	}
}

public class DB_schema  {
	private int recordNum=0;
	private int fieldNum;
	private String[] fieldtype;
	private String[] fieldName;
	public DB_schema(int fieldnum, String[] fieldname, String[] fieldtype) { //input instructor param by event handler
		this.fieldNum = fieldnum;
		this.fieldtype = fieldtype;
		this.fieldName = fieldname;
	}	
	public void init() {
		recordNum = 0;
		fieldNum = 0;
		Arrays.fill(fieldtype,"");
		//add JTable initialize
	}		
	public int getfieldNum() {
		return this.fieldNum;
	}
	public void addfieldNum() {
		this.fieldNum++;
	}
	public String getfieldtype(int fieldIdx) {
		return this.fieldtype[fieldIdx];
	}
	public String[] getfieldtype() {
		return this.fieldtype;
	}
	public String getfieldname(int fieldIdx) {
		return this.fieldName[fieldIdx];
	}
	public String[] getfieldname() {
		return this.fieldName;
	}
	public int getrecordNum() {
		return this.recordNum;
	}
	public void addrecordNum() {
		this.recordNum++;
	}
}

class DBM extends DB_schema {
	public DBM(int fieldnum, String[] fieldname, String[] fieldtype) {
		super(fieldnum, fieldname, fieldtype);
	}
	ArrayList<Record> record = new ArrayList<Record>();
	private int currentkeynum = 0;
	private int sortfieldIdx = 0;
	
	public void addRecord(){
		if(record.size()==0) {		
			this.record.add(new Record(super.getfieldNum(), super.getfieldname(), super.getfieldtype()));
			super.addrecordNum();
		}
		else if(record.get(record.size()-1).fieldContainer.size() >= super.getfieldNum()) {//record is exist, but empty
			this.record.add(new Record(super.getfieldNum(), super.getfieldname(), super.getfieldtype()));
			super.addrecordNum();
		}
		else System.out.println("previous record is empty, cant add before full record"); //error 
	}
	
	public void addValue(String inputstr) {
		if(record.isEmpty()) addRecord();
		if(currentkeynum==4) {
			currentkeynum=0;
			addRecord(); 
		}
		if(record.size()==1 && currentkeynum<super.getfieldNum()) {
			record.get(0).fieldContainer.add(new FieldType(super.getfieldtype(currentkeynum),super.getfieldname(currentkeynum),inputstr));
			currentkeynum++;
		}
		else {
			//System.out.println("error");
			record.get(super.getrecordNum()-1).fieldContainer.add(new FieldType(super.getfieldtype(currentkeynum),super.getfieldname(currentkeynum),inputstr));
			currentkeynum++;
		}
	}
	
	public void setSortfield(int fieldIdx) {
		this.sortfieldIdx = fieldIdx;
		for(int i=0;i<record.size();i++) {
			record.get(i).setSortfield(sortfieldIdx);
		}
		Collections.sort(record);
	}
}

class Record implements Comparable <Record>{
	ArrayList<FieldType> fieldContainer = new ArrayList<FieldType>();
	private String[] fieldname;
	private String[] fieldtype;
	private int sortfield;
	public Record(int fieldnum, String[] fieldname, String[] fieldtype){
		this.fieldname = fieldname; 
		this.fieldtype = fieldtype;
	}
	@Override
	public int compareTo(Record o) {
		// TODO Auto-generated method stub
		int targetlength = o.fieldContainer.get(sortfield).getkeyaluelen();
		if(targetlength == fieldContainer.get(sortfield).getkeyaluelen()) return 0;
		else if(targetlength < fieldContainer.get(sortfield).getkeyaluelen()) return 1;
		else return -1;
	}
	public void setSortfield(int i) {
		this.sortfield = i;
	}
}
 
class FieldType{	
	private String strvalue = "";
	private String keytype;
	private String fieldName;
	private int valuelen;
	FieldType(String fieldtype, String fieldname, String inputstr) {
		this.keytype = fieldtype;
		this.fieldName = fieldname;
		this.strvalue = inputstr;
	}
	public Object getvalue(){
		Object rtnvalue = null;
		try {
			switch (this.keytype) {
				case "STRING": rtnvalue = strvalue;
				break;
				case "CHAR"  : rtnvalue = strvalue.charAt(0);
				break;
				case "INT"   : rtnvalue =  Integer.parseInt(strvalue);
				break;
				case "DOUBLE"  : rtnvalue = Double.parseDouble(strvalue);
				break;
			}
			return rtnvalue;
		}catch(Exception e) {
			e.getMessage();
		}
		return rtnvalue;
	}
		
	public int getkeyaluelen() {
		valuelen = getvalue().toString().length();
		return valuelen;
	}
}