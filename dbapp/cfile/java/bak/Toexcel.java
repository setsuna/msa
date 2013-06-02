import java.io.*;
import java.nio.charset.Charset;
import java.util.ArrayList;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFFont;
import org.apache.poi.hssf.usermodel.HSSFRichTextString;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.util.CellRangeAddress;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import com.csvreader.CsvReader;
@SuppressWarnings("deprecation")
public class Toexcel{
	public static <LabeledCSVParser> void main(String args[]){
		
		String group_name=args[1];
	    String user_name=args[2];
	    String servic_name=args[3];
	    String sDate=args[4];
	    String flow_name=args[5];
		int flag=Integer.parseInt(args[0]);
	    		
		FileOutputStream out = null;
		try {
			out = new FileOutputStream("/datacenter/msa/report/msaexport/srvrecord/srvexport.xls");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		//workbook url
		Workbook wb = new HSSFWorkbook();
		Sheet s = wb.createSheet();
		
		Row r = null;
		Cell c = null;
		wb.setSheetName(0, "MSA" );
		
		//表格头样式
		HSSFCellStyle normalStyle = (HSSFCellStyle) wb.createCellStyle();  
		normalStyle.setBorderLeft(HSSFCellStyle.BORDER_THIN); 
		normalStyle.setBorderTop(HSSFCellStyle.BORDER_THIN); 
		normalStyle.setBorderRight(HSSFCellStyle.BORDER_THIN);  
		normalStyle.setBorderBottom(HSSFCellStyle.BORDER_THIN);  
		normalStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);  
		normalStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);  
		normalStyle.setWrapText(true);
		
		
		//数据内容样式
		HSSFCellStyle normalStyle2 = (HSSFCellStyle) wb.createCellStyle();  
		normalStyle2.setBorderLeft(HSSFCellStyle.BORDER_THIN); 
		normalStyle2.setBorderTop(HSSFCellStyle.BORDER_THIN); 
		normalStyle2.setBorderRight(HSSFCellStyle.BORDER_THIN);  
		normalStyle2.setBorderBottom(HSSFCellStyle.BORDER_THIN);  
		normalStyle2.setAlignment(HSSFCellStyle.ALIGN_CENTER);  
		normalStyle2.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);  
		normalStyle2.setWrapText(true);
		
			
		//第一行样式
		HSSFCellStyle normalStyle3 = (HSSFCellStyle) wb.createCellStyle();  
		normalStyle3.setBorderLeft(HSSFCellStyle.BORDER_THIN); 
		normalStyle3.setBorderTop(HSSFCellStyle.BORDER_THIN); 
		normalStyle3.setBorderRight(HSSFCellStyle.BORDER_THIN);  
		normalStyle3.setBorderBottom(HSSFCellStyle.BORDER_THIN);  
		normalStyle3.setAlignment(HSSFCellStyle.ALIGN_CENTER);  
		normalStyle3.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);  
		normalStyle3.setWrapText(true);
		
		//表头内容样式
		HSSFCellStyle normalStyle4 = (HSSFCellStyle) wb.createCellStyle();  
		normalStyle4.setBorderLeft(HSSFCellStyle.BORDER_THIN); 
		normalStyle4.setBorderTop(HSSFCellStyle.BORDER_THIN); 
		normalStyle4.setBorderRight(HSSFCellStyle.BORDER_THIN);  
		normalStyle4.setBorderBottom(HSSFCellStyle.BORDER_THIN);  
		normalStyle4.setAlignment(HSSFCellStyle.ALIGN_LEFT);  
		normalStyle4.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);  
		normalStyle4.setWrapText(true);
		
		HSSFFont font = (HSSFFont) wb.createFont();
		font.setFontName("黑体");
		font.setFontHeightInPoints((short) 16);//设置字体大小
		normalStyle3.setFont(font);//选择需要用到的字体格式

		HSSFFont font2 = (HSSFFont) wb.createFont();
		font2.setFontName("仿宋_GB2312");
		font2.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);//粗体显示
		font2.setFontHeightInPoints((short) 12);
		normalStyle.setFont(font2);//选择需要用到的字体格式

		int rownum;
		for (rownum = (short) 0; rownum < flag; rownum++){
		        r = s.createRow(rownum);
		        r.setHeight((short) 350);//目的是想把行高设置成20px
			    for (short cellnum = (short) 0; cellnum < 9; cellnum++){  
			    	     c = r.createCell(cellnum);       
			              try {     
						       ArrayList<String[]> csvList = new ArrayList<String[]>(); 
						       String csvFilePath = "/datacenter/msa/report/msaexport/srvrecord/srvexport.csv";  
						       CsvReader reader = new CsvReader(csvFilePath,',',Charset.forName("UTF8"));           
						       
						       while(reader.readRecord()){    
						           csvList.add(reader.getValues());  
						       }              
						       reader.close();                    
				               String  cell = csvList.get(rownum)[cellnum]; 
				               c.setCellValue(cell);		
				               c.setCellStyle(normalStyle2);
				               System.out.println(cell);                   
						  }catch(Exception ex){  
						      ex.printStackTrace();
						  } 	
			    }
		}
		
		
	     int starRow = -1;
	     int rows = 10;
	     int rowHeight1=500;
	     int rowHeight2=450;
	     
	     s.shiftRows(starRow + 1, s.getLastRowNum(), rows,true,false);
	     
	     starRow = starRow + 11;//starRow=9 第10行
	     for (int i = 0; i < rows; i++) {
	         HSSFRow sourceRow = null;
	         HSSFRow targetRow = null;
	         HSSFCell targetCell = null;
	         short m;
	         //starRow = starRow + 1;//starRow=10 第11行
	         sourceRow = (HSSFRow) s.getRow(starRow);//获取第11行
	         targetRow = (HSSFRow) s.createRow(i);  
	         targetRow.setHeight(sourceRow.getHeight());		         
	         for (m = sourceRow.getFirstCellNum(); m < sourceRow.getLastCellNum(); m++) {
		         targetCell = targetRow.createCell(m);
		         
		         if(i==0){//第一行
		        	   targetRow.setHeight((short)rowHeight1);        	
		        	   CellRangeAddress Hebin=new CellRangeAddress(0,0,0,8);
			 	       s.addMergedRegion(Hebin);
			 	       targetCell.setCellValue(new HSSFRichTextString("金盾MSA行为管理系统"));	   
			 	       targetCell.setCellStyle(normalStyle3); 
		         }
		         
		         if(i==1){//第二行
		        	 if(m==0){
		        		   targetCell.setCellValue("报表名称");		
		        		   targetCell.setCellStyle(normalStyle);
		        	 }else{
		        		   CellRangeAddress Hebin=new CellRangeAddress(1,1,1,8);
				 	       s.addMergedRegion(Hebin);
				 	       targetCell.setCellValue(new HSSFRichTextString("服务汇总记录查询"));	   
				 	       targetCell.setCellStyle(normalStyle4); 
		        	 }
		         }
		        	 
		         if(i==2){//第三行
		        	 if(m==0){
		        		   targetCell.setCellValue("查询日期");		
		        		   targetCell.setCellStyle(normalStyle);
		        	 }else{
		        		   CellRangeAddress Hebin=new CellRangeAddress(2,2,1,8);
				 	       s.addMergedRegion(Hebin);
				 	       targetCell.setCellValue(new HSSFRichTextString(sDate));	   
				 	       targetCell.setCellStyle(normalStyle4); 
		        	 }
		        	
		         }
		         
		         if(i==3){//第四行	  
		        	   targetRow.setHeight((short)rowHeight1);    
		        	   CellRangeAddress Hebin=new CellRangeAddress(3,3,0,8);
			 	       s.addMergedRegion(Hebin);
			 	       targetCell.setCellValue(new HSSFRichTextString("过滤条件"));	   
			 	       targetCell.setCellStyle(normalStyle); 
		         }
		         
		         if(i==4){//第五行
		        	 if(m==0){
		        		   targetCell.setCellValue("部门名称");		
		        		   targetCell.setCellStyle(normalStyle);
		        	 }else{
		        		   CellRangeAddress Hebin=new CellRangeAddress(4,4,1,8);
				 	       s.addMergedRegion(Hebin);
				 	       targetCell.setCellValue(new HSSFRichTextString(group_name));	   
				 	       targetCell.setCellStyle(normalStyle4); 
		        	 }
		         }
		         
		         if(i==5){//第六行
		        	 if(m==0){
		        		 targetCell.setCellValue("用户名");		
		        		 targetCell.setCellStyle(normalStyle);
		        	 }else{
		        		   CellRangeAddress Hebin=new CellRangeAddress(5,5,1,8);
				 	       s.addMergedRegion(Hebin);
				 	       targetCell.setCellValue(new HSSFRichTextString(user_name));	   
				 	       targetCell.setCellStyle(normalStyle4); 
		        	 }		        	
		         }
		         
		         
		         if(i==6){//第八行
		        	 if(m==0){
		        		 targetCell.setCellValue("服务类型");		
		        		 targetCell.setCellStyle(normalStyle);
		        	 }else{
		        		 CellRangeAddress Hebin=new CellRangeAddress(6,6,1,8);
				 	     s.addMergedRegion(Hebin);
				 	     targetCell.setCellValue(new HSSFRichTextString(servic_name));	   
				 	     targetCell.setCellStyle(normalStyle4);  
	        	 }		        	
		         }
		         
		         if(i==7){//第9行
		        	 if(m==0){
		        		 targetCell.setCellValue("流量范围");		
		        		 targetCell.setCellStyle(normalStyle);
		        	 }else{
		        		 CellRangeAddress Hebin=new CellRangeAddress(7,7,1,8);
				 	     s.addMergedRegion(Hebin);
				 	     targetCell.setCellValue(new HSSFRichTextString(flow_name));	   
				 	     targetCell.setCellStyle(normalStyle4); 
		        	 }		        	
		         }
		         
		         if(i==8){//第10行	 
		        	 targetRow.setHeight((short)rowHeight1);    
		        	 CellRangeAddress Hebin=new CellRangeAddress(8,8,0,8);
			 	     s.addMergedRegion(Hebin);
			 	     targetCell.setCellValue(new HSSFRichTextString("查询结果"));	   
			 	     targetCell.setCellStyle(normalStyle); 
		         }
		         
		         if(i==9){
		        	 
		        		if(m==0){
			        		targetCell.setCellValue("序号");
			        	}
			        	if(m==1){
			        		targetCell.setCellValue("用户名");
			        	}
			        	
			        	if(m==2){
			        		targetCell.setCellValue("部门");
			        	}
			        	if(m==3){
			        		targetCell.setCellValue("日期");
			        	}
			        	
			        	if(m==4){
			        		targetCell.setCellValue("服务");
			        	}
			        	if(m==5){
			        		targetCell.setCellValue("上流量");
			        	}
			        	
			        	if(m==6){
			        		targetCell.setCellValue("下流量");
			        	}
			        	if(m==7){
			        		targetCell.setCellValue("连接次数");
			        	}
			        	
			        	if(m==8){
			        		targetCell.setCellValue("连接时长");
			        	}
			        	targetCell.setCellStyle(normalStyle); 
			        	targetRow.setHeight((short)rowHeight2);    
		         }
		     
	         }
	         
	         
	     }
		
	    for(int i=0;i<9;i++){
			s.setColumnWidth((short)i, 4000); //第一个参数代表列id(从0开始),第2个参数代表宽度值
		}
		
	   

		try {
			wb.write(out);
		} catch (IOException e) {
			e.printStackTrace();
		}
		try {
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		}
}