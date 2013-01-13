#pragma once
#include"about.h"
#include "DirectxCheck.h"

namespace Starsv4
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	/// <summary> 
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class Form1 : public System::Windows::Forms::Form
	{	
	public:
		Form1(void)
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
	private: System::Windows::Forms::GroupBox *  groupBox1;
	private: System::Windows::Forms::CheckBox *  planets;
	private: System::Windows::Forms::GroupBox *  groupBox2;
	private: System::Windows::Forms::HScrollBar *  speed;
	private: System::Windows::Forms::GroupBox *  groupBox3;



	private: System::Windows::Forms::GroupBox *  groupBox4;
	private: System::Windows::Forms::RadioButton *  lowdensity;
	private: System::Windows::Forms::RadioButton *  meddensity;
	private: System::Windows::Forms::RadioButton *  highdensity;
	private: System::Windows::Forms::GroupBox *  groupBox5;
	private: System::Windows::Forms::RadioButton *  lowquality;
	private: System::Windows::Forms::RadioButton *  medquality;
	private: System::Windows::Forms::RadioButton *  highquality;
	private: System::Windows::Forms::RadioButton *  lowtexture;
	private: System::Windows::Forms::RadioButton *  medtexture;
	private: System::Windows::Forms::RadioButton *  hightexture;
	private: System::Windows::Forms::GroupBox *  groupBox6;
	private: System::Windows::Forms::CheckBox *  widescreen;
	private: System::Windows::Forms::CheckBox *  music;
	private: System::Windows::Forms::CheckBox *  framerate;
	private: System::Windows::Forms::Button *  ok;
	private: System::Windows::Forms::Button *  cancel;
	private: System::Windows::Forms::Button *  about;
	private: System::Windows::Forms::LinkLabel *  linkLabel1;
	private: Microsoft::Win32::RegistryKey* regkey;
	private: System::Windows::Forms::LinkLabel*  linkLabel2;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container * components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>


		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager*  resources = (new System::ComponentModel::ComponentResourceManager(__typeof(Form1)));
			this->groupBox1 = (new System::Windows::Forms::GroupBox());
			this->planets = (new System::Windows::Forms::CheckBox());
			this->groupBox2 = (new System::Windows::Forms::GroupBox());
			this->speed = (new System::Windows::Forms::HScrollBar());
			this->groupBox3 = (new System::Windows::Forms::GroupBox());
			this->highdensity = (new System::Windows::Forms::RadioButton());
			this->meddensity = (new System::Windows::Forms::RadioButton());
			this->lowdensity = (new System::Windows::Forms::RadioButton());
			this->groupBox4 = (new System::Windows::Forms::GroupBox());
			this->highquality = (new System::Windows::Forms::RadioButton());
			this->medquality = (new System::Windows::Forms::RadioButton());
			this->lowquality = (new System::Windows::Forms::RadioButton());
			this->groupBox5 = (new System::Windows::Forms::GroupBox());
			this->hightexture = (new System::Windows::Forms::RadioButton());
			this->medtexture = (new System::Windows::Forms::RadioButton());
			this->lowtexture = (new System::Windows::Forms::RadioButton());
			this->groupBox6 = (new System::Windows::Forms::GroupBox());
			this->framerate = (new System::Windows::Forms::CheckBox());
			this->music = (new System::Windows::Forms::CheckBox());
			this->widescreen = (new System::Windows::Forms::CheckBox());
			this->ok = (new System::Windows::Forms::Button());
			this->cancel = (new System::Windows::Forms::Button());
			this->about = (new System::Windows::Forms::Button());
			this->linkLabel1 = (new System::Windows::Forms::LinkLabel());
			this->linkLabel2 = (new System::Windows::Forms::LinkLabel());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->planets);
			this->groupBox1->Location = System::Drawing::Point(20, 14);
			this->groupBox1->Name = S"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(160, 83);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = S"Planets";
			// 
			// planets
			// 
			this->planets->Location = System::Drawing::Point(13, 35);
			this->planets->Name = S"planets";
			this->planets->Size = System::Drawing::Size(134, 20);
			this->planets->TabIndex = 0;
			this->planets->Text = S"Enable Planets";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->speed);
			this->groupBox2->Location = System::Drawing::Point(193, 14);
			this->groupBox2->Name = S"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(340, 83);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = S"Speed";
			// 
			// speed
			// 
			this->speed->Location = System::Drawing::Point(13, 28);
			this->speed->Minimum = 1;
			this->speed->Name = S"speed";
			this->speed->Size = System::Drawing::Size(307, 27);
			this->speed->TabIndex = 0;
			this->speed->Value = 40;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->highdensity);
			this->groupBox3->Controls->Add(this->meddensity);
			this->groupBox3->Controls->Add(this->lowdensity);
			this->groupBox3->Location = System::Drawing::Point(20, 118);
			this->groupBox3->Name = S"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(160, 139);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = S"Starfield Density";
			// 
			// highdensity
			// 
			this->highdensity->Location = System::Drawing::Point(13, 97);
			this->highdensity->Name = S"highdensity";
			this->highdensity->Size = System::Drawing::Size(87, 21);
			this->highdensity->TabIndex = 2;
			this->highdensity->Text = S"High";
			// 
			// meddensity
			// 
			this->meddensity->Location = System::Drawing::Point(13, 62);
			this->meddensity->Name = S"meddensity";
			this->meddensity->Size = System::Drawing::Size(87, 21);
			this->meddensity->TabIndex = 1;
			this->meddensity->Text = S"Medium";
			// 
			// lowdensity
			// 
			this->lowdensity->Location = System::Drawing::Point(13, 28);
			this->lowdensity->Name = S"lowdensity";
			this->lowdensity->Size = System::Drawing::Size(87, 21);
			this->lowdensity->TabIndex = 0;
			this->lowdensity->Text = S"Low";
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->highquality);
			this->groupBox4->Controls->Add(this->medquality);
			this->groupBox4->Controls->Add(this->lowquality);
			this->groupBox4->Location = System::Drawing::Point(193, 118);
			this->groupBox4->Name = S"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(160, 139);
			this->groupBox4->TabIndex = 3;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = S"Render Quality";
			// 
			// highquality
			// 
			this->highquality->Location = System::Drawing::Point(13, 97);
			this->highquality->Name = S"highquality";
			this->highquality->Size = System::Drawing::Size(87, 21);
			this->highquality->TabIndex = 2;
			this->highquality->Text = S"High";
			// 
			// medquality
			// 
			this->medquality->Location = System::Drawing::Point(13, 62);
			this->medquality->Name = S"medquality";
			this->medquality->Size = System::Drawing::Size(87, 21);
			this->medquality->TabIndex = 1;
			this->medquality->Text = S"Medium ";
			// 
			// lowquality
			// 
			this->lowquality->Location = System::Drawing::Point(13, 28);
			this->lowquality->Name = S"lowquality";
			this->lowquality->Size = System::Drawing::Size(87, 21);
			this->lowquality->TabIndex = 0;
			this->lowquality->Text = S"Low";
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->hightexture);
			this->groupBox5->Controls->Add(this->medtexture);
			this->groupBox5->Controls->Add(this->lowtexture);
			this->groupBox5->Location = System::Drawing::Point(373, 118);
			this->groupBox5->Name = S"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(160, 139);
			this->groupBox5->TabIndex = 4;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = S"Texture Quality";
			// 
			// hightexture
			// 
			this->hightexture->Location = System::Drawing::Point(13, 97);
			this->hightexture->Name = S"hightexture";
			this->hightexture->Size = System::Drawing::Size(87, 21);
			this->hightexture->TabIndex = 2;
			this->hightexture->Text = S"High";
			this->hightexture->CheckedChanged += new System::EventHandler(this, &Form1::radioButton9_CheckedChanged);
			// 
			// medtexture
			// 
			this->medtexture->Location = System::Drawing::Point(13, 62);
			this->medtexture->Name = S"medtexture";
			this->medtexture->Size = System::Drawing::Size(87, 21);
			this->medtexture->TabIndex = 1;
			this->medtexture->Text = S"Medium";
			// 
			// lowtexture
			// 
			this->lowtexture->Location = System::Drawing::Point(13, 28);
			this->lowtexture->Name = S"lowtexture";
			this->lowtexture->Size = System::Drawing::Size(87, 21);
			this->lowtexture->TabIndex = 0;
			this->lowtexture->Text = S"Low";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->framerate);
			this->groupBox6->Controls->Add(this->music);
			this->groupBox6->Controls->Add(this->widescreen);
			this->groupBox6->Location = System::Drawing::Point(20, 277);
			this->groupBox6->Name = S"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(513, 77);
			this->groupBox6->TabIndex = 5;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = S"Other Options";
			// 
			// framerate
			// 
			this->framerate->Location = System::Drawing::Point(366, 35);
			this->framerate->Name = S"framerate";
			this->framerate->Size = System::Drawing::Size(137, 20);
			this->framerate->TabIndex = 2;
			this->framerate->Text = S"Display Frame Rate";
			// 
			// music
			// 
			this->music->Location = System::Drawing::Point(187, 35);
			this->music->Name = S"music";
			this->music->Size = System::Drawing::Size(141, 20);
			this->music->TabIndex = 1;
			this->music->Text = S"Enable Music";
			// 
			// widescreen
			// 
			this->widescreen->Location = System::Drawing::Point(13, 35);
			this->widescreen->Name = S"widescreen";
			this->widescreen->Size = System::Drawing::Size(155, 20);
			this->widescreen->TabIndex = 0;
			this->widescreen->Text = S"Widescreen Display";
			// 
			// ok
			// 
			this->ok->Location = System::Drawing::Point(20, 374);
			this->ok->Name = S"ok";
			this->ok->Size = System::Drawing::Size(80, 42);
			this->ok->TabIndex = 6;
			this->ok->Text = S"OK";
			this->ok->Click += new System::EventHandler(this, &Form1::ok_Click);
			// 
			// cancel
			// 
			this->cancel->Location = System::Drawing::Point(113, 374);
			this->cancel->Name = S"cancel";
			this->cancel->Size = System::Drawing::Size(80, 42);
			this->cancel->TabIndex = 7;
			this->cancel->Text = S"Cancel";
			this->cancel->Click += new System::EventHandler(this, &Form1::cancel_Click);
			// 
			// about
			// 
			this->about->Location = System::Drawing::Point(207, 374);
			this->about->Name = S"about";
			this->about->Size = System::Drawing::Size(80, 42);
			this->about->TabIndex = 8;
			this->about->Text = S"About";
			this->about->Click += new System::EventHandler(this, &Form1::about_Click);
			// 
			// linkLabel1
			// 
			this->linkLabel1->LinkArea = System::Windows::Forms::LinkArea(10, 22);
			this->linkLabel1->Location = System::Drawing::Point(307, 374);
			this->linkLabel1->Name = S"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(206, 20);
			this->linkLabel1->TabIndex = 9;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = S"website - www.syntaxcritical.com";
			this->linkLabel1->UseCompatibleTextRendering = true;
			this->linkLabel1->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &Form1::linkLabel1_LinkClicked);
			// 
			// linkLabel2
			// 
			this->linkLabel2->AutoEllipsis = true;
			this->linkLabel2->LinkArea = System::Windows::Forms::LinkArea(12, 49);
			this->linkLabel2->Location = System::Drawing::Point(307, 394);
			this->linkLabel2->Name = S"linkLabel2";
			this->linkLabel2->Size = System::Drawing::Size(226, 22);
			this->linkLabel2->TabIndex = 10;
			this->linkLabel2->TabStop = true;
			this->linkLabel2->Text = S"donations - https://www.paypal.com/cgi-bin/webscr\?cmd=_xclick";
			this->linkLabel2->UseCompatibleTextRendering = true;
			this->linkLabel2->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &Form1::linkLabel2_LinkClicked);
			// 
			// Form1
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(546, 436);
			this->Controls->Add(this->linkLabel2);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->about);
			this->Controls->Add(this->cancel);
			this->Controls->Add(this->ok);
			this->Controls->Add(this->groupBox6);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Icon = (__try_cast<System::Drawing::Icon*  >(resources->GetObject(S"$this.Icon")));
			this->Name = S"Form1";
			this->Text = S"Stars V4.5";
			this->Load += new System::EventHandler(this, &Form1::Form1_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox4->ResumeLayout(false);
			this->groupBox5->ResumeLayout(false);
			this->groupBox6->ResumeLayout(false);
			this->ResumeLayout(false);

		}	
	private: System::Void Form1_Load(System::Object *  sender, System::EventArgs *  e)
			 {
				 this->planets->Checked = false;
				 this->speed->Value = 50;
				 this->framerate->Checked = false;
				 this->lowdensity->Checked = true;
				 this->meddensity->Checked = false;
				 this->highdensity->Checked = false;
				 this->lowtexture->Checked = true;
				 this->medtexture->Checked = false;
				 this->hightexture->Checked = false;
				 this->lowquality->Checked = true;
				 this->medquality->Checked = false;
				 this->highquality->Checked = false;
				 this->widescreen->Checked = false;
				 this->music->Checked = false;
				 regkey = Microsoft::Win32::Registry::CurrentUser;
				 regkey = regkey->OpenSubKey("Software");
				 regkey = regkey->OpenSubKey("Eric Duhon");
				 if(regkey != NULL)
				 {
					 regkey = regkey->OpenSubKey("Stars v4.5");
					 if(regkey != NULL)
					 {

						Int32* planets = dynamic_cast<Int32*>(regkey->GetValue("planets"));				 
						if(planets != NULL)
						{
							if((*planets) == 1) this->planets->Checked = true;
							else this->planets->Checked = false;
						}
						Int32* speed = dynamic_cast<Int32*>(regkey->GetValue("speed"));				 
						if(speed != NULL)
							this->speed->Value = (*speed);
						Int32* frame_counter = dynamic_cast<Int32*>(regkey->GetValue("frame_counter"));				 
						if(frame_counter != NULL)
						{
							if((*frame_counter) == 1) this->framerate->Checked = true;
							else this->framerate->Checked = false;
						}
						Int32* star_density = dynamic_cast<Int32*>(regkey->GetValue("star_density"));				 
						if(star_density != NULL)
						{
							if((*star_density) == 0) this->lowdensity->Checked = true;
							else this->lowdensity->Checked = false;
							if((*star_density) == 1) this->meddensity->Checked = true;
							else this->meddensity->Checked = false;
							if((*star_density) == 2) this->highdensity->Checked = true;
							else this->highdensity->Checked = false;
						}
						Int32* texture_quality = dynamic_cast<Int32*>(regkey->GetValue("texture_quality"));				 
						if(texture_quality != NULL)
						{
							if((*texture_quality) == 0) this->lowtexture->Checked = true;
							else this->lowtexture->Checked = false;
							if((*texture_quality) == 1) this->medtexture->Checked = true;
							else this->medtexture->Checked = false;
							if((*texture_quality) == 2) this->hightexture->Checked = true;
							else this->hightexture->Checked = false;
						}
						Int32* quality = dynamic_cast<Int32*>(regkey->GetValue("quality"));				 
						if(quality != NULL)
						{
							if((*quality) == 0) this->lowquality->Checked = true;
							else this->lowquality->Checked = false;
							if((*quality) == 1) this->medquality->Checked = true;
							else this->medquality->Checked = false;
							if((*quality) == 2) this->highquality->Checked = true;
							else this->highquality->Checked = false;
						}
						Int32* widescreen = dynamic_cast<Int32*>(regkey->GetValue("widescreen"));				 
						if(widescreen != NULL)
						{
							if((*widescreen) == 1) this->widescreen->Checked = true;
							else this->widescreen->Checked = false;
						}
						Int32* music = dynamic_cast<Int32*>(regkey->GetValue("music"));				 
						if(music != NULL)
						{
							if((*music) == 1) this->music->Checked = true;
							else this->music->Checked = false;
						}
					 }
				 }
				
				 if(DirectxCheck::checkDirectx9())
				 {
					 if(!DirectxCheck::checkHighTexture())
					 {
						 if(!DirectxCheck::checkMedTexture())
						 {
							 this->medtexture->set_Enabled(false);
							 this->hightexture->set_Enabled(false);
						 }
						 else
						 {
							 this->hightexture->set_Enabled(false);
						 }
					 }
					 if(!DirectxCheck::CheckShadder20())
					 {
						 if(!DirectxCheck::CheckShader11())
						 {
							 this->medquality->set_Enabled(false);
							 this->highquality->set_Enabled(false);
						 }
						 else
						 {
							 this->highquality->set_Enabled(false);
						 }
					 }
				 }

        

			 }

			 	private: void  SaveReg()
			 {
				 regkey = Microsoft::Win32::Registry::CurrentUser;
				 regkey = regkey->CreateSubKey("Software");
				 regkey = regkey->CreateSubKey("Eric Duhon");
				 regkey = regkey->CreateSubKey("Stars v4.5");
				 Int32 planets = this->planets->Checked;
				 regkey->SetValue("planets",__box(planets));				 
				 Int32 speed = this->speed->Value;
				 regkey->SetValue("speed",__box(speed));				 
				 Int32 frame_counter = this->framerate->Checked;
				 regkey->SetValue("frame_counter",__box(frame_counter));				 
				 Int32 star_density;
				 if(this->lowdensity->Checked) star_density = 0;
				 if(this->meddensity->Checked) star_density = 1;
				 if(this->highdensity->Checked) star_density = 2;
				 regkey->SetValue("star_density",__box(star_density));				 
				 
				 Int32 texture_quality;
				 if(this->lowtexture->Checked) texture_quality = 0;
				 if(this->medtexture->Checked) texture_quality = 1;
				 if(this->hightexture->Checked) texture_quality = 2;
				 regkey->SetValue("texture_quality",__box(texture_quality));				 
				 
				 Int32 quality;
				 if(this->lowquality->Checked) quality = 0;
				 if(this->medquality->Checked) quality = 1;
				 if(this->highquality->Checked) quality = 2;
				 regkey->SetValue("quality",__box(quality));				 
		
				 Int32 widescreen = this->widescreen->Checked;
				 regkey->SetValue("widescreen",__box(widescreen));				 

				 Int32 music = this->music->Checked;
				 regkey->SetValue("music",__box(music));				 

			 }

private: System::Void radioButton9_CheckedChanged(System::Object *  sender, System::EventArgs *  e)
		 {
		 }

private: System::Void about_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 About* about = new About();
			 about->ShowDialog(this);
		 }

private: System::Void cancel_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 this->Close();
		 }

private: System::Void ok_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 SaveReg();
			 this->Close();
		 }

private: System::Void linkLabel1_LinkClicked(System::Object *  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *  e)
		 {
			 System::Diagnostics::Process::Start( "www.syntaxcritical.com" );

		 }

private: System::Void linkLabel2_LinkClicked(System::Object*  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs*  e) {
			 System::Diagnostics::Process::Start( "https://www.paypal.com/cgi-bin/webscr?cmd=_xclick&business=duhone%40gmail%2ecom&item_name=Stars%204%2e5&no_shipping=0&no_note=1&tax=0&currency_code=USD&bn=PP%2dDonationsBF&charset=UTF%2d8" );

		 }
};
}


