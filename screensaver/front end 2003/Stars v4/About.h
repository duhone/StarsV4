#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Starsv4
{
	/// <summary> 
	/// Summary for About
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class About : public System::Windows::Forms::Form
	{
	public: 
		About(void)
		{
			InitializeComponent();
		}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Button *  button1;
	private: System::Windows::Forms::Label *  label1;

	private: System::Windows::Forms::Label *  label3;
	private: System::Windows::Forms::Label *  label4;
	private: System::Windows::Forms::LinkLabel *  linkLabel1;
	private: System::Windows::Forms::Label *  label5;
	private: System::Windows::Forms::Label*  label2;
	private: System::Windows::Forms::LinkLabel*  linkLabel2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager*  resources = (new System::ComponentModel::ComponentResourceManager(__typeof(About)));
			this->button1 = (new System::Windows::Forms::Button());
			this->label1 = (new System::Windows::Forms::Label());
			this->label3 = (new System::Windows::Forms::Label());
			this->label4 = (new System::Windows::Forms::Label());
			this->linkLabel1 = (new System::Windows::Forms::LinkLabel());
			this->label5 = (new System::Windows::Forms::Label());
			this->label2 = (new System::Windows::Forms::Label());
			this->linkLabel2 = (new System::Windows::Forms::LinkLabel());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(112, 224);
			this->button1->Name = S"button1";
			this->button1->Size = System::Drawing::Size(64, 32);
			this->button1->TabIndex = 0;
			this->button1->Text = S"OK";
			this->button1->Click += new System::EventHandler(this, &About::button1_Click);
			// 
			// label1
			// 
			this->label1->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				(System::Byte)0));
			this->label1->Location = System::Drawing::Point(88, 24);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(104, 24);
			this->label1->TabIndex = 1;
			this->label1->Text = S"Stars V4.5";
			// 
			// label3
			// 
			this->label3->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				(System::Byte)0));
			this->label3->Location = System::Drawing::Point(89, 81);
			this->label3->Name = S"label3";
			this->label3->Size = System::Drawing::Size(146, 16);
			this->label3->TabIndex = 3;
			this->label3->Text = S"created by Eric Duhon";
			// 
			// label4
			// 
			this->label4->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				(System::Byte)0));
			this->label4->Location = System::Drawing::Point(89, 97);
			this->label4->Name = S"label4";
			this->label4->Size = System::Drawing::Size(146, 24);
			this->label4->TabIndex = 4;
			this->label4->Text = S"music by Billy Garretson";
			// 
			// linkLabel1
			// 
			this->linkLabel1->Location = System::Drawing::Point(89, 57);
			this->linkLabel1->Name = S"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(168, 24);
			this->linkLabel1->TabIndex = 5;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = S"www.syntaxcritical.com";
			this->linkLabel1->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &About::linkLabel1_LinkClicked);
			// 
			// label5
			// 
			this->label5->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				(System::Byte)0));
			this->label5->Location = System::Drawing::Point(192, 32);
			this->label5->Name = S"label5";
			this->label5->Size = System::Drawing::Size(80, 16);
			this->label5->TabIndex = 6;
			this->label5->Text = S"© 2006";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(89, 132);
			this->label2->Name = S"label2";
			this->label2->Size = System::Drawing::Size(183, 52);
			this->label2->TabIndex = 7;
			this->label2->Text = S"Click Link below to donate money, \r\nto help cover costs so that I may\r\ncontinue t" 
				S"o create free screensavers.\r\nThank You";
			// 
			// linkLabel2
			// 
			this->linkLabel2->AutoEllipsis = true;
			this->linkLabel2->LinkArea = System::Windows::Forms::LinkArea(0, 31);
			this->linkLabel2->Location = System::Drawing::Point(89, 194);
			this->linkLabel2->Name = S"linkLabel2";
			this->linkLabel2->Size = System::Drawing::Size(168, 17);
			this->linkLabel2->TabIndex = 8;
			this->linkLabel2->TabStop = true;
			this->linkLabel2->Text = S"https://www.paypal.com/cgi-bin/webscr\?cmd=_xclick";
			this->linkLabel2->UseCompatibleTextRendering = true;
			this->linkLabel2->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &About::linkLabel2_LinkClicked);
			// 
			// About
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(293, 277);
			this->Controls->Add(this->linkLabel2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Icon = (__try_cast<System::Drawing::Icon*  >(resources->GetObject(S"$this.Icon")));
			this->Name = S"About";
			this->Text = S"About";
			this->Load += new System::EventHandler(this, &About::About_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}		
	private: System::Void About_Load(System::Object *  sender, System::EventArgs *  e)
			 {
			 }

	private: System::Void button1_Click(System::Object *  sender, System::EventArgs *  e)
			 {
				 Close();
			 }

	

	

private: System::Void linkLabel1_LinkClicked(System::Object*  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs*  e) {
			 System::Diagnostics::Process::Start( "www.syntaxcritical.com" );

		 }
private: System::Void linkLabel2_LinkClicked(System::Object*  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs*  e) {
			  System::Diagnostics::Process::Start( "https://www.paypal.com/cgi-bin/webscr?cmd=_xclick&business=duhone%40gmail%2ecom&item_name=Stars%204%2e5&no_shipping=0&no_note=1&tax=0&currency_code=USD&bn=PP%2dDonationsBF&charset=UTF%2d8" );

		 }
};
}